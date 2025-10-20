#include "Raindrop/Modules/Render/RenderGraph/RenderGraphSetupStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Modules/Render/Stages/RenderInfo.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/RunnableGraph.hpp>

namespace Raindrop::Render{
    void RenderGraphSetupStage::initialize(Scheduler::StageInitHelper& helper) {
        _loop = helper.loop();
        _engine = &helper.engine();

        _core = helper.modules().getModuleAs<RenderCoreModule>("RenderCore");
    }

    void RenderGraphSetupStage::shutdown(){
        if (_core->device().waitIdle() != vk::Result::eSuccess){
            spdlog::error("Failed to wait device idle");
        }
        
        _runnableGraphs.clear();
    }

    const char* RenderGraphSetupStage::name() const{
        return "RenderGraphRender";
    }

    void RenderGraphSetupStage::invalidate(){
        for (auto& graph : _runnableGraphs){
            graph.invalid = true;
        }
    }

    Scheduler::StageResult RenderGraphSetupStage::execute(){
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
                return StageResult::Skip("Cannot get render graph module");
            }

            renderGraph.frameGraph = renderGraphModule->createGraph();
        }

        // if pending recompile is true, compile the graph
        if (renderGraph.pendingRecompile){
            invalidate();
            renderGraph.pendingRecompile = false;
        }

        return StageResult::Continue();
    }
}