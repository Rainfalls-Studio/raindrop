#include "Raindrop/Modules/Render/RenderGraph/RenderGraphRenderStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Engine.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/RunnableGraph.hpp>

namespace Raindrop::Render{
    void RenderGraphRenderStage::initialize(Scheduler::StageInitHelper& helper) {
        _loop = helper.loop();
        _engine = &helper.engine();
        
        helper.registerHook(Scheduler::Hook{
            Scheduler::Phase::RENDER,
            "Render graph render",
            [this]{return render();}
        });
    }

    Scheduler::HookResult RenderGraphRenderStage::render(){
        using namespace Scheduler;

        // auto& preRenderResult = _loop.getOrEmplaceStorage<PreRenderResult>();
        // auto& renderGraph = _loop.emplaceStorage<RenderGraph>();

        // // get render graph
        // auto renderGraphModule = _weakRenderGraphModule.lock();

        // // if the reference is not valid, try to get it again from the module manager
        // if (!renderGraphModule){
        //     auto& modules = _engine->getModuleManager();

        //     renderGraphModule = modules.getModuleAs<RenderGraphModule>("RenderGraph");
        //     _weakRenderGraphModule = renderGraphModule;
            
        //     // If there are no render graph modules registred
        //     if (!renderGraphModule){
        //         spdlog::error("Cannot get render graph module");
        //         return HookResult::Skip("Cannot get render graph module");
        //     }
        // }

        // // if pending recompile is true, compile the graph
        // if (renderGraph._pendingRecompile){
        //     // renderGraph._frameGraph->compile(renderGraphModule->context());

        //     renderGraph._pendingRecompile = false;
        // }

        // if (!preRenderResult.available){
        //     return HookResult::Skip("pre render is not available");
        // }

        return HookResult::Continue();
    }
}