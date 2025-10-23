#include "Raindrop/Modules/Render/RenderGraph/RenderSequencePass.hpp"

namespace Raindrop::Render{
    RenderSequencePass::RenderSequencePass(
        const crg::FramePass& pass,
        crg::GraphContext& context,
        crg::RunnableGraph& graph,
        std::vector<std::unique_ptr<IRenderStep>> steps,
        crg::ru::Config ruConfig,
        GetPassIndexCallback passIndex
    ) : 
        RunnablePass{
            pass,
            context,
            graph,
            {
                InitialiseCallback([this](uint32_t index){initialize(index);}),
                GetPipelineStateCallback( [](){ return crg::getPipelineState(VK_PIPELINE_STAGE_TRANSFER_BIT ); } ),
                [this]( crg::RecordContext & recContext, VkCommandBuffer cb, uint32_t i ){ recordInto( recContext, cb, i ); },
                std::move( passIndex ),
                IsEnabledCallback([this]{return isEnabled();})
            },
            std::move( ruConfig )
        },
        _steps{std::move(steps)}
    {}

    void RenderSequencePass::recordInto(
        crg::RecordContext&,
        VkCommandBuffer commandBuffer,
        uint32_t
    ){
        for (const auto& step : _steps){
            step->record(commandBuffer);
        }
    }

    void RenderSequencePass::initialize(uint32_t){}
    bool RenderSequencePass::isEnabled(){
        return !_steps.empty();
    }
}