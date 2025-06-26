#pragma once

#include "Raindrop/Window/Property.hpp"
#include "Swapchain.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>

namespace Raindrop::Graphics::Backend::Vulkan{
    class WindowProperty : public Raindrop::Window::Property{
        public:
            VkSurfaceKHR surface;
            std::unique_ptr<Swapchain> swapchain;
            
    };
}