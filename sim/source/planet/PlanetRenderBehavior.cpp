#include "PlanetRenderBehavior.hpp"
#include "PlanetComponent.hpp"
#include "PlanetServiceBehavior.hpp"

using Raindrop::Components::Transform;
using Raindrop::Components::Camera;
using Raindrop::Behaviors::FrameSnapshotService;

using Raindrop::Scene::Scene;

namespace Planet{
    struct PushConstant{
        glm::mat4 transform;
        glm::mat4 viewTransform;
    };



    PreRenderBehavior::PreRenderBehavior(std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx) : _cmdCtx(cmdCtx){

    }

    void PreRenderBehavior::initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene){
        _scene = &scene;

        _planetService = scene.getBehaviorIndex<ServiceBehavior>();
        _snapshotService = scene.getBehaviorIndex<FrameSnapshotService>();


        if (_snapshotService != FrameSnapshotService::INVALID_SLOT_ID){
            auto snapshotService = scene.getBehavior<FrameSnapshotService>(_snapshotService);
            _planetSlot = snapshotService->registerSlot<RenderDataPayload>();
        }

        auto& modules = engine.getModuleManager();

        _core = modules.getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
    }

    void PreRenderBehavior::shutdown(){}

    void PreRenderBehavior::execute(){
        if (_planetService == Raindrop::Scene::INVALID_BEHAVIOR_ID) return;

        auto cmdCtx = _cmdCtx.lock();
        if (!cmdCtx) return;
        
        auto service = _scene->getBehavior<ServiceBehavior>(_planetService);
        if (!service) return;

        auto view = _scene->registry().view<PlanetComponent, Transform>();
        if (view.size_hint() == 0) return;

        auto frameSnapshot = _scene->getBehavior<FrameSnapshotService>(_snapshotService);
        if (!frameSnapshot) return;

        if (_planetSlot == FrameSnapshotService::INVALID_SLOT_ID) return;


        const auto& planetOffsets = frameSnapshot->readSlotOffsets(_planetSlot);

        for (auto planetOffset : planetOffsets){
            const auto& planet = frameSnapshot->read<RenderDataPayload>(planetOffset);

            if (planet.id == INVALID_PLANET_ID) continue;

            // we want to write the chunk instance ids
            const auto& chunks = frameSnapshot->readPtr<ChunkDataPayload>(planet.chunks);

            const auto& runtime = service->getRuntimeData(planet.id);

            if (!runtime) continue;
            if (!runtime->valid) continue;
            

            if (!runtime->render.has_value()){
                runtime->render.emplace(service->constructPlanetRenderData());
            }

            const auto& renderData = runtime->render.value();

            PlanetRenderData::Instance* mapped = static_cast<PlanetRenderData::Instance*>(renderData.instances.mapped);
            // uint32_t offset = 0;

            for (uint32_t i=0; i<planet.chunkCount; i++){
                auto& chunk = chunks[i];
                uint32_t index = chunk.instanceIndex;

                auto& map = mapped[index];

                map.face = static_cast<uint32_t>(chunk.face);
                map.offsetUV = chunk.uvMin;
                map.sizeUV = chunk.uvScale;
                
                map.packedNeighbors = 
                    (static_cast<uint32_t>(chunk.neighorLOD[0]) << 0)  |
                    (static_cast<uint32_t>(chunk.neighorLOD[1]) << 8)  |
                    (static_cast<uint32_t>(chunk.neighorLOD[2]) << 16) |
                    (static_cast<uint32_t>(chunk.neighorLOD[3]) << 24);

                // memcpy(void *__restrict dest, const void *__restrict src, size_t n)
            }
        }
    }

    const char* PreRenderBehavior::name() const{
        return "Planet - Pre-render";
    }


    RenderBehavior::RenderBehavior(std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx) :
        _output{output},
        _cmdCtx(cmdCtx)
    {}

    void RenderBehavior::initialize(Raindrop::Engine& engine, Scene& scene){
        _scene = &scene;

        _planetService = scene.getBehaviorIndex<ServiceBehavior>();
        _snapshotService = scene.getBehaviorIndex<FrameSnapshotService>();

        if (_snapshotService != FrameSnapshotService::INVALID_SLOT_ID){
            auto snapshotService = scene.getBehavior<FrameSnapshotService>(_snapshotService);

            _planetSlot = snapshotService->registerSlot<RenderDataPayload>();
            _cameraSlot = snapshotService->registerSlot<Raindrop::Components::Camera::RenderData>();
        }

        auto& modules = engine.getModuleManager();

        _core = modules.getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
        _assets = modules.getModuleAs<Raindrop::Asset::AssetModule>("Asset");

        _fragmentShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.frag.glsl");
        _vertexShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.vert.glsl");
        
        auto result = _fragmentShader->load().and_then([&]{return _vertexShader->load();});
        if (!result) spdlog::info("{}", result.error().message());


        createPipelineLayout();
        createPipeline();
    }

    void RenderBehavior::shutdown(){
        auto device = _core->deviceManager().device();

        if (_pipeline){
            device.destroyPipeline(_pipeline);
        }

        if (_pipelineLayout){
            device.destroyPipelineLayout(_pipelineLayout);
        }
    }

    void RenderBehavior::execute(){

        if (_planetService == Raindrop::Scene::INVALID_BEHAVIOR_ID) return;

        auto output = _output.lock();
        if (!output) return;
        if (!output->wasAcquired()) return;

        auto cmdCtx = _cmdCtx.lock();
        if (!cmdCtx) return;
        
        auto service = _scene->getBehavior<ServiceBehavior>(_planetService);
        if (!service) return;

        auto view = _scene->registry().view<PlanetComponent, Transform>();
        if (view.size_hint() == 0) return;

        auto frameSnapshot = _scene->getBehavior<FrameSnapshotService>(_snapshotService);
        if (!frameSnapshot) return;

        if (_planetSlot == FrameSnapshotService::INVALID_SLOT_ID) return;


        // ---- Command setup ----------------------------------------------------------------------


        vk::CommandBuffer cmd = cmdCtx->currentBuffer().cmdBuffer;

        auto extent = output->extent();
        vk::Viewport viewport{
            0.f, 0.f,
            static_cast<float>(extent.width),
            static_cast<float>(extent.height),
            0.f, 1.f
        };

        cmd.setViewport(0, viewport);

        vk::Rect2D scissor{
            {},
            extent
        };

        cmd.setScissor(0, scissor);

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline);

        
        const auto& planetOffsets = frameSnapshot->readSlotOffsets(_planetSlot);
        const auto& cameraOffsets = frameSnapshot->readSlotOffsets(_cameraSlot);

        for (const auto cameraOffset : cameraOffsets){
            const auto& camera = frameSnapshot->read<Raindrop::Components::Camera::RenderData>(cameraOffset);

            for (const auto planetOffset : planetOffsets){
                const RenderDataPayload& planet = frameSnapshot->read<RenderDataPayload>(planetOffset);

                if (planet.id == INVALID_PLANET_ID) continue;

                const auto& runtime = service->getRuntimeData(planet.id);

                if (!runtime) continue;
                if (!runtime->valid) continue;
                

                if (!runtime->render.has_value()){
                    runtime->render.emplace(service->constructPlanetRenderData());
                }

                const auto& renderData = runtime->render.value();

                const vk::Buffer& indexBuffer = renderData.grid.index.buffer;
                const auto& indexCount = renderData.grid.index.count;

                const vk::Buffer& vertexBuffer = renderData.grid.vertex.buffer;
                const vk::Buffer& instanceBuffer = renderData.instances.buffer;

                PushConstant pc{
                    planet.transform,
                    camera.viewProjection
                };

                cmd.pushConstants<PushConstant>(_pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, pc);
                cmd.bindVertexBuffers(0, {vertexBuffer, instanceBuffer}, {0, 0});
                cmd.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);

                auto chunkCount = planet.chunkCount;
                const auto& chunks = frameSnapshot->readPtr<ChunkDataPayload>(planet.chunks);


                for (uint32_t i=0; i<chunkCount; i++){
                    const auto& chunk = chunks[i];

                    uint32_t instanceIndex = chunk.instanceIndex;
                    cmd.drawIndexed(indexCount, 1, 0, 0, instanceIndex);
                }
            }
        }
    }

    void RenderBehavior::createPipelineLayout(){

        vk::PushConstantRange range(
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(PushConstant)
        );

        vk::PipelineLayoutCreateInfo info(
            {},
            0, nullptr,
            1, &range
        );

        auto device = _core->deviceManager().device();

        if (auto result = device.createPipelineLayout(info); result.result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create pipeline layout");
        } else {
            _pipelineLayout = result.value;
        }
    }

    void RenderBehavior::createPipeline(){
        vk::PipelineShaderStageCreateInfo stages[] = {
            _vertexShader->stageInfo(),
            _fragmentShader->stageInfo()
        };

        vk::VertexInputBindingDescription inputBindings[] = {
            PlanetRenderData::Vertex::description(),
            PlanetRenderData::Instance::description(1),
        };

        std::vector<vk::VertexInputAttributeDescription> inputAttributes = PlanetRenderData::Vertex::attributes();
        {
            auto instanceDescription = PlanetRenderData::Instance::attributes(2, 1);

            inputAttributes.insert(
                inputAttributes.end(),
                instanceDescription.begin(),
                instanceDescription.end()
            );
        }

        vk::PipelineVertexInputStateCreateInfo inputStage(
            {},
            inputBindings,
            inputAttributes
        );

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
            {},
            vk::PrimitiveTopology::eTriangleList,
            vk::False
        };

        vk::PipelineViewportStateCreateInfo viewportStateInfo{
            {},
            1, nullptr,
            1, nullptr
        };

        vk::PipelineRasterizationStateCreateInfo rasterInfo{
            {},
            vk::False,
            vk::False,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eNone,
            vk::FrontFace::eCounterClockwise,
            vk::True,
            0.f,
            0.f,
            0.f,
            1.f
        };

        vk::PipelineMultisampleStateCreateInfo multisampleInfo{
            {},
            vk::SampleCountFlagBits::e1,
            vk::False,
            0.f,
            nullptr,
            vk::False,
            vk::False
        };

        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo{
            {},
            vk::True,
            vk::True,
            vk::CompareOp::eLess,
            vk::False,
            vk::False,
            {},
            {},
            0.f,
            1.f
        };

        vk::PipelineColorBlendAttachmentState attachments[] = {
            vk::PipelineColorBlendAttachmentState{
                vk::False,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                {
                    vk::ColorComponentFlagBits::eR |
                    vk::ColorComponentFlagBits::eG |
                    vk::ColorComponentFlagBits::eB |
                    vk::ColorComponentFlagBits::eA
                }
            }
        };

        vk::PipelineColorBlendStateCreateInfo colorBlendInfo(
            {},
            vk::False,
            vk::LogicOp::eCopy,
            attachments,
            {}
        );

        vk::DynamicState dynamicStates[] = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicStateInfo(
            {},
            dynamicStates
        );

        auto output = _output.lock();
        if (!output){
            throw std::runtime_error("No output");
        }

        vk::GraphicsPipelineCreateInfo info(
            {},
            stages,
            &inputStage,
            &inputAssembly,
            nullptr,
            &viewportStateInfo,
            &rasterInfo,
            &multisampleInfo,
            &depthStencilInfo,
            &colorBlendInfo,
            &dynamicStateInfo,
            _pipelineLayout,
            output->renderPass(),
            0,
            {},
            {}
        );

        auto device = _core->deviceManager().device();

        if (auto result = device.createGraphicsPipeline({}, info); result.result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create pipeline");
        } else {
            _pipeline = result.value;
        }
    }

    const char* RenderBehavior::name() const{
        return "Planet Render";
    }
}