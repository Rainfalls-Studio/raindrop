#pragma once

#include "../RenderTargetImpl.hpp"
#include "Raindrop/Graphics/Internal/Vulkan/SwapchainAttachment.hpp"
#include "Swapchain.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    class SwapchainRenderTarget : public RenderTargetImpl{
        public:
            SwapchainRenderTarget(std::weak_ptr<Swapchain> swapchain);

            virtual std::shared_ptr<AttachmentImpl> getAttachment(const std::string& name) override;
            virtual std::shared_ptr<AttachmentImpl> addAttachment(const std::string& name) override;
        
        private:
            std::weak_ptr<Swapchain> _swapchain;
            std::shared_ptr<SwapchainAttachment> _colorAttachment;
    };
}