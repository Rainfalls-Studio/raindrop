#include "PlanetRenderBehavior.hpp"
#include "PlanetComponent.hpp"
#include "PlanetServiceBehavior.hpp"

using Raindrop::Components::Transform;
using Raindrop::Components::Camera;

using Raindrop::Scene::Scene;

namespace Planet{
    struct PushConstant{
        glm::mat4 transform;
        glm::mat4 viewTransform;
    };

    RenderBehavior::RenderBehavior(std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx) :
        _output{output},
        _cmdCtx(cmdCtx)
    {}

    void RenderBehavior::initialize(Raindrop::Engine& engine, Scene& scene){
        _scene = &scene;

        _planetService = scene.getBehaviorIndex<ServiceBehavior>();

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

        if (_planetService == INVALID_PLANET_ID) return;

        auto output = _output.lock();
        if (!output) return;
        if (!output->wasAcquired()) return;

        auto cmdCtx = _cmdCtx.lock();
        if (!cmdCtx) return;
        
        auto service = _scene->getBehavior<ServiceBehavior>(_planetService);
        if (!service) return;

        auto view = _scene->registry().view<PlanetComponent, Transform>();
        if (view.size_hint() == 0) return;


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


        glm::mat4 viewTransform = glm::mat4(1.f);
        {
            auto camera = _scene->registry().view<Camera, Transform>().front();    
            if (_scene->isValid(camera)){
                const glm::mat4& transform = _scene->getComponent<Transform>(camera).worldTransform;
                const glm::mat4& projection = _scene->getComponent<Camera>(camera).getProjectionMatrix();

                viewTransform = projection * glm::inverse(transform);
            }
        }

        for (auto e : view){
            auto [planet, transform] = view.get(e);

            if (planet.id == INVALID_PLANET_ID){
                planet.id = service->createPlanet();
            }

            RuntimePlanet* planetRuntime = service->getRuntimeData(planet.id);
            if (!planetRuntime){
                continue;
            }

            // skip invalid planets and planets that don't have render data
            if (!planetRuntime->valid){
                continue;
            }

            if (!planetRuntime->render.has_value()){
                planetRuntime->render.emplace(service->constructPlanetRenderData());
            }

            const auto& renderData = planetRuntime->render.value();

            const vk::Buffer& indexBuffer = renderData.grid.index.buffer;
            const auto& indexCount = renderData.grid.index.count;

            const vk::Buffer& vertexBuffer = renderData.grid.vertex.buffer;
            // const auto& vertexCount = renderData.grid.vertex.count;


            PushConstant pc{
                transform.worldTransform,
                viewTransform
            };

            cmd.pushConstants<PushConstant>(_pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, pc);

            cmd.bindVertexBuffers(0, vertexBuffer, {0});
            cmd.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);
            cmd.drawIndexed(indexCount, 1, 0, 0, 0);
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

        vk::VertexInputBindingDescription inputBinding = PlanetRenderData::Vertex::description();
        auto inputAttributes = PlanetRenderData::Vertex::attributes();

        vk::PipelineVertexInputStateCreateInfo inputStage(
            {},
            inputBinding,
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
}