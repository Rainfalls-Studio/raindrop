#pragma once

#include "../AttachmentImpl.hpp"
#include "Swapchain.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    class SwapchainAttachment : public AttachmentImpl{
        public:
            SwapchainAttachment(std::weak_ptr<Swapchain> swapchain);
            ~SwapchainAttachment();

        private:
            std::weak_ptr<Swapchain> _swapchain;

    };
}