#pragma once

#include <vulkan/vulkan.hpp>

namespace Raindrop::Graphics{
    struct WindowContext{
        vk::UniqueSurfaceKHR surface;
        vk::UniqueSwapchainKHR swapchain;

        struct ImageInfo{
            vk::Image image;
            vk::UniqueSemaphore imageFinished;
            vk::UniqueSemaphore imageAvailable;
            vk::UniqueFence inFlightFence;
            vk::UniqueFence imageInFlight;
        };

        std::vector<ImageInfo> images;
        uint32_t _currentImageIndex = 0;

        vk::Extent2D extent;
        vk::SurfaceFormatKHR surfaceFormat;
        vk::PresentModeKHR presentMode;
    };
}