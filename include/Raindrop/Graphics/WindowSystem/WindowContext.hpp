#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>
#include "Raindrop/Window/Window.hpp"
#include "WindowOutputTarget.hpp"

namespace Raindrop::Graphics::WindowSystem{
    struct WindowContext{
        std::weak_ptr<Window::Window> window;
        vk::SurfaceKHR surface;
        vk::SurfaceCapabilitiesKHR capabilities;
        vk::SwapchainKHR swapchain;

        struct ImageInfo{
            vk::Image image;
            vk::Semaphore imageFinished;
            vk::Semaphore imageAvailable;
            vk::Fence inFlightFence;
            vk::Fence imageInFlight;
        };

        std::vector<ImageInfo> images;
        uint32_t currentImageIndex = 0;

        vk::Extent2D extent;
        vk::SurfaceFormatKHR surfaceFormat;
        vk::PresentModeKHR presentMode;

        std::shared_ptr<WindowOutputTarget> target;
    };
}