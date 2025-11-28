#pragma once

#include "Raindrop/Modules/Render/RenderGraph/IRenderStep.hpp"
#include "IRenderOutput.hpp"

namespace Raindrop::Render{
    class RenderOutputEndRenderPass : public IRenderStep{
        public:
            RenderOutputEndRenderPass(
                SharedRenderOutput renderOutput
            );

            virtual void record(vk::CommandBuffer cmd) override;
        
        private:
            SharedRenderOutput _renderOutput;
    };
}