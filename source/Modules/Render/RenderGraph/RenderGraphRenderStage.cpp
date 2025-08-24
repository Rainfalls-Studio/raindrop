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
        
        helper.registerHook(Scheduler::Hook{
            Scheduler::Phase::RENDER,
            "Render graph render",
            [this]{return render();}
        });

        helper.registerHook(
            Scheduler::Hook{
                Scheduler::Phase::PRE_RENDER_SETUP,
                "Render graph setup",
                [this]{return setupFrameGraph();}
            }
        );
    }

    void RenderGraphRenderStage::invalidate(){
        for (auto& graph : _runnableGraphs){
            graph.invalid = true;
        }
    }

    Scheduler::HookResult RenderGraphRenderStage::setupFrameGraph(){
        using namespace Scheduler;

        auto& renderGraph = _loop.getOrEmplaceStorage<RenderGraph>();

        // get render graph
        auto renderGraphModule = _weakRenderGraphModule.lock();

        // if the reference is not valid, try to get it again from the module manager
        if (!renderGraphModule){
            auto& modules = _engine->getModuleManager();

            renderGraphModule = modules.getModuleAs<RenderGraphModule>("RenderGraph");
            _weakRenderGraphModule = renderGraphModule;

            // rebuild all graphs.
            // We don't use _runnnableGraphs.clear() as some resources may still be on the fly
            invalidate();
            
            // If there are no render graph modules registred
            if (!renderGraphModule){
                spdlog::error("Cannot get render graph module");
                return HookResult::Skip("Cannot get render graph module");
            }

            renderGraph.frameGraph = renderGraphModule->createGraph();
        }

        // if pending recompile is true, compile the graph
        if (renderGraph.pendingRecompile){
            invalidate();
            renderGraph.pendingRecompile = false;
        }

        return HookResult::Continue();
    }

    Scheduler::HookResult RenderGraphRenderStage::render(){
        using namespace Scheduler;

        auto& info = _loop.getOrEmplaceStorage<RenderInfo>();
        auto& renderGraph = _loop.getOrEmplaceStorage<RenderGraph>();


        auto renderGraphModule = _weakRenderGraphModule.lock();

        if (!renderGraphModule){
            return HookResult::Skip("No render graph module");
        }
        
        // if not available, just skip a frame
        if (!info.available){
            return HookResult::Skip("pre render is not available");
        }

        // ! The if frameCount is less than runnableGraphs.size(). There may be resources in use destroyed
        // TODO: fix
        if (info.frameCount != _runnableGraphs.size()){
            _runnableGraphs.resize(info.frameCount);
        }

        auto& graph = _runnableGraphs[info.currentFrame];
        spdlog::info("current : {}", info.currentFrame);

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

        return HookResult::Continue();
    }
}