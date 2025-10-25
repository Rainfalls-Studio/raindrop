#pragma once

#include "Raindrop/Modules/Render/RenderGraph/IRenderStep.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "IRenderOutput.hpp"

namespace Raindrop::Render{
    class RenderOutputBeginRenderPass : public IRenderStep{
        public:
            RenderOutputBeginRenderPass(
                SharedRenderOutput renderOutput,
                std::shared_ptr<RenderGraph> _renderGraph
            );

            virtual void record(vk::CommandBuffer cmd) override;
        
        private:
            SharedRenderOutput _renderOutput;
            std::weak_ptr<RenderGraph> _renderGraph;
    };
}