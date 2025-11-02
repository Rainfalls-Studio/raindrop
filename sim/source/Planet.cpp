#include "Planet.hpp"

Planet::RenderStage::RenderStage(std::weak_ptr<Planet> planet, std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx) : 
    _planet{planet},
    _output{output},
    _cmdCtx(cmdCtx)
{}

const char* Planet::RenderStage::name() const{
    return "Planet - Render Stage";
}

void Planet::RenderStage::initialize(Raindrop::Scheduler::StageInitHelper&){}
void Planet::RenderStage::shutdown(){}

Raindrop::Scheduler::StageResult Planet::RenderStage::execute(){
    using namespace Raindrop::Scheduler;
    auto output = _output.lock();

    if (!output){
        return StageResult::Skip("No output");
    }

    auto cmdCtx = _cmdCtx.lock();

    if (!cmdCtx){
        return StageResult::Skip("No render command context");
    }

    auto planet = _planet.lock();

    if (!planet){
        return StageResult::Skip("No planet to render");
    }

    // planet->_pipeline

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

    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, planet->_pipeline);
    cmd.draw(3, 1, 0, 0);

    return StageResult::Continue();
}

Planet::Planet(Raindrop::Engine& engine, std::shared_ptr<Raindrop::Render::IRenderOutput> output){
    _output = output;
    auto& modules = engine.getModuleManager();

    _core = modules.getModuleAs<Raindrop::Render::RenderCoreModule>("RenderCore");
    // if (!_core){

    // }
    _assets = modules.getModuleAs<Raindrop::Asset::AssetModule>("Asset");

    _fragmentShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.frag.glsl");
    _vertexShader = _assets->loadOrGet<Raindrop::Render::Shader>("{resources}/shaders/planet.vert.glsl");

    auto result = _fragmentShader->load().and_then([&]{return _vertexShader->load();});

    if (!result) spdlog::info("{}", result.error().message());

    createPipelineLayout();
    createPipeline();
}

Planet::~Planet(){
    auto device = _core->device();

    if (_pipeline){
        device.destroyPipeline(_pipeline);    
    }

    if (_pipelineLayout){
        device.destroyPipelineLayout(_pipelineLayout);
    }
}

void Planet::createPipelineLayout(){
    vk::PipelineLayoutCreateInfo info{
        {},
        0, nullptr,
        0, nullptr,
    };

    auto device = _core->device();

    if (auto result = device.createPipelineLayout(info); result.result != vk::Result::eSuccess){
        throw std::runtime_error("Failed to create pipeline layout");
    } else {
        _pipelineLayout = result.value;
    }
}


void Planet::createPipeline(){

    vk::PipelineShaderStageCreateInfo stages[] = {
        _vertexShader->stageInfo(),
        _fragmentShader->stageInfo()
    };

    vk::PipelineVertexInputStateCreateInfo inputStage{
        {},
        0, nullptr,
        0, nullptr
    };

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
        {},
        vk::PrimitiveTopology::eTriangleList,
        vk::False
    };

    vk::PipelineTessellationStateCreateInfo tessellationInfo{
        {},
        0
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
        _output->renderPass(),
        0,
        {},
        {}
    );

    auto device = _core->device();

    if (auto result = device.createGraphicsPipeline({}, info); result.result != vk::Result::eSuccess){
        throw std::runtime_error("Failed to create pipeline");
    } else {
        _pipeline = result.value;
    }
}

