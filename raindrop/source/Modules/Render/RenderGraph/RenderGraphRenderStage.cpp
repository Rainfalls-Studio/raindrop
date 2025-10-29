#include "Raindrop/Modules/Render/RenderGraph/RenderGraphRenderStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Modules/Render/Stages/RenderInfo.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/RunnableGraph.hpp>

namespace Raindrop::Render{
    RenderGraphRenderStage::RenderGraphRenderStage(std::weak_ptr<RenderGraph> graph) : 
        _graph{graph}
    {}


    void RenderGraphRenderStage::initialize(Scheduler::StageInitHelper& helper) {
        _loop = helper.loop();
        _engine = &helper.engine();

        _core = helper.modules().getModuleAs<RenderCoreModule>("RenderCore");
    }

    void RenderGraphRenderStage::shutdown(){
        if (_core->device().waitIdle() != vk::Result::eSuccess){
            spdlog::error("Failed to wait device idle");
        }
    }

    const char* RenderGraphRenderStage::name() const{
        return "RenderGraphRender";
    }

    Scheduler::StageResult RenderGraphRenderStage::execute(){
        using namespace Scheduler;

        auto graph = _graph.lock();

        if (!graph){
            spdlog::warn("The render graph is not valid !");
            return StageResult::Skip("The provided render graph is not valid");
        }

        auto& info = _loop.getOrEmplaceStorage<RenderInfo>();
        
        // if not available, just skip a frame
        if (!info.available){

            // if a fram is skipped. The sync objects needs to be rebuilt (signaled semaphore has not been waited on)
            // TODO: invalidate sync objects instead of whole graph
            graph->invalidate();
            
            return StageResult::Skip("image is not available");
        }

    
        
        auto& queue = _core->graphicsQueue();

        auto renderFinished = graph->run(*queue, info.imageAvailable);

        if (renderFinished.success){
            info.renderFinishedFence = renderFinished.fence;
            info.renderFinishedSemaphore = renderFinished.semaphore;
        } else {
            info.renderFinishedFence = VK_NULL_HANDLE;
            info.renderFinishedSemaphore = VK_NULL_HANDLE;
        }

        return StageResult::Continue();
    }
}