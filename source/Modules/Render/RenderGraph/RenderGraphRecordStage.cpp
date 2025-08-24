#include "Raindrop/Modules/Render/RenderGraph/RenderGraphRecordStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Modules/Render/RenderOutput/BlitToRenderOutputPass.hpp"

namespace Raindrop::Render{
    RenderGraphRecordStage::RenderGraphRecordStage(Layers::Layer layer) : _layer{layer}{}

    void RenderGraphRecordStage::initialize(Scheduler::StageInitHelper& helper){
        _loop = helper.loop();
        _engine = &helper.engine();

        helper.registerHook(Scheduler::Hook{
            Scheduler::Phase::PRE_RENDER,
            "Render graph record",
            [this]{ return render();}
        });
    }

    Scheduler::HookResult RenderGraphRecordStage::render(){
        using namespace Scheduler;
        
        RenderGraph& renderGraph = _loop.getOrEmplaceStorage<RenderGraph>();

        if (!_submited){
            auto& frameGraph = renderGraph.frameGraph;

            if (!frameGraph){
                return HookResult::Skip("No framegraph");
            }
            
            frameGraph->createPass("WindowClear", crg::RunnablePassCreator(
                [this](const crg::FramePass& pass, crg::GraphContext& ctx, crg::RunnableGraph& graph) -> crg::RunnablePassPtr {
                    return std::make_unique<BlitToRenderOutputPass>(
                        *_engine,
                        pass,
                        ctx,
                        graph,
                        "main"
                    );
                }
            ));
            _submited = true;
        }

        return HookResult::Continue();
    }
}