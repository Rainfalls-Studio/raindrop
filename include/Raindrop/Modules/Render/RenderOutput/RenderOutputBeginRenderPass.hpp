#pragma once

#include "Raindrop/Modules/Render/RenderGraph/IRenderStep.hpp"
#include "IRenderOutput.hpp"

namespace Raindrop::Render{
    class RenderOutputBeginRenderPass : public IRenderStep{
        public:
            RenderOutputBeginRenderPass(
                SharedRenderOutput renderOutput
            );

            virtual void record(vk::CommandBuffer cmd) override;
        
        private:
            SharedRenderOutput _renderOutput;
    };
}