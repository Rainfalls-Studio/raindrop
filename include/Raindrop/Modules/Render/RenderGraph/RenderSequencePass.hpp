#pragma once

#include <list>
#include <RenderGraph/RunnableGraph.hpp>
#include "IRenderStep.hpp"

namespace Raindrop::Render{

    class RenderSequencePass : public crg::RunnablePass{
        public:
            RenderSequencePass(
                const crg::FramePass& pass,
                crg::GraphContext& context,
                crg::RunnableGraph& graph,
                std::vector<std::unique_ptr<IRenderStep>> steps,
                crg::ru::Config ruConfig = {},
                GetPassIndexCallback passIndex = GetPassIndexCallback([]{return 0u;})
            );
        
        private:
            std::vector<std::unique_ptr<IRenderStep>> _steps;

            void recordInto(
                crg::RecordContext& context,
                VkCommandBuffer commandBuffer,
                uint32_t index
            );

            void initialize(uint32_t passIndex);
            bool isEnabled();
    };
}