#include "Raindrop/Modules/Render/RenderGraph/RenderGraphRenderStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Modules/Render/Stages/RenderInfo.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/RunnableGraph.hpp>

namespace Raindrop::Render{
    void RenderGraphRenderStage::initialize(Scheduler::StageInitHelper& helper) {
        _loop = helper.loop();
        _engine = &helper.engine();

        _core = helper.modules().getModuleAs<RenderCoreModule>("RenderCore");
        _weakRenderGraphModule = helper.modules().getModuleAs<RenderGraphModule>("RenderGraph");
    }

    void RenderGraphRenderStage::shutdown(){
        if (_core->device().waitIdle() != vk::Result::eSuccess){
            spdlog::error("Failed to wait device idle");
        }
        
        _runnableGraphs.clear();
    }

    const char* RenderGraphRenderStage::name() const{
        return "RenderGraphRender";
    }

    void RenderGraphRenderStage::invalidate(){
        for (auto& graph : _runnableGraphs){
            graph.invalid = true;
        }
    }

    Scheduler::StageResult RenderGraphRenderStage::execute(){
        using namespace Scheduler;

        auto& info = _loop.getOrEmplaceStorage<RenderInfo>();
        auto& renderGraph = _loop.getOrEmplaceStorage<RenderGraph>();


        auto renderGraphModule = _weakRenderGraphModule.lock();

        if (!renderGraphModule){
            return StageResult::Skip("No render graph module");
        }
        
        // if not available, just skip a frame
        if (!info.available){
            return StageResult::Skip("image is not available");
        }

        // ! The if frameCount is less than runnableGraphs.size(). There may be resources in use destroyed
        // TODO: fix
        if (info.frameCount != _runnableGraphs.size()){
            _runnableGraphs.resize(info.frameCount);
        }

        auto& graph = _runnableGraphs[info.currentFrame];

        if (graph.invalid){
            try{
                graph.graph = renderGraph.frameGraph->compile(renderGraphModule->context());
                graph.invalid = false;
            } catch (...){} 
        }

        crg::SemaphoreWait submitWait{
            info.imageAvailable,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
        };

        auto& queue = _core->graphicsQueue();

        auto renderFinished = graph.graph->run(
            {submitWait},
            *queue
        );

        info.renderFinishedFence = graph.graph->getFence();
        info.renderFinishedSemaphore = renderFinished[0].semaphore;

        return StageResult::Continue();
    }
}