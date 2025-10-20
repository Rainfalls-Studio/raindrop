#include "Raindrop/Modules/Render/RenderGraph/RenderGraphRecordStage.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Modules/Render/RenderOutput/BlitToRenderOutputPass.hpp"

namespace Raindrop::Render{
    RenderGraphRecordStage::RenderGraphRecordStage(Layers::Layer layer) : _layer{layer}{}

    void RenderGraphRecordStage::shutdown(){}

    const char* RenderGraphRecordStage::name() const{
        return "RenderGraphRecord";
    }


    void RenderGraphRecordStage::initialize(Scheduler::StageInitHelper& helper){
        _loop = helper.loop();
        _engine = &helper.engine();
    }

    Scheduler::StageResult RenderGraphRecordStage::execute(){
        using namespace Scheduler;
        
        RenderGraph& renderGraph = _loop.getOrEmplaceStorage<RenderGraph>();

        if (!_submited){
            auto& frameGraph = renderGraph.frameGraph;

            if (!frameGraph){
                return StageResult::Skip("No framegraph");
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

        return StageResult::Continue();
    }
}