#pragma once

#include "Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp"
#include "Raindrop/Modules/Render/RenderGraph/IRenderStep.hpp"
#include "ImGuiModule.hpp"

namespace Raindrop::Render{
    class ImGuiRenderStep : public IRenderStep{
        public:
            ImGuiRenderStep(
                std::shared_ptr<ImGuiModule> module,
                SharedRenderOutput renderOutput,
                IRenderOutput::Name name
            );

            virtual void record(vk::CommandBuffer cmd) override;
        
        private:
            std::shared_ptr<ImGuiModule> _module;
            SharedRenderOutput _renderOutput;
            IRenderOutput::Name _name;
    };
}