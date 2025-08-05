#pragma once

#include <RenderGraph/RunnableGraph.hpp>
#include "../IRenderOutput.hpp"

namespace Raindrop::Render{
    class BlitToRenderOutputPass : public crg::RunnablePass{
        public:
            BlitToRenderOutputPass(
                Engine& engine,
                const crg::FramePass& pass,
                crg::GraphContext& context,
                crg::RunnableGraph& graph,
                const IRenderOutput::Name& renderOutput,
                crg::ru::Config ruConfig = {},
                GetPassIndexCallback passIndex = GetPassIndexCallback([]{return 0u;})
            );
        
        private:
            Engine* _engine;
            IRenderOutput::Name _renderOutputName;
            SharedRenderOutput _renderOutputLock;
            
            SharedRenderOutput findRenderOutput();
            void recordInto(
                crg::RecordContext& context,
                VkCommandBuffer commandBuffer,
                uint32_t index
            );

            void initialize(uint32_t passIndex);
            bool isEnabled();
    };
}