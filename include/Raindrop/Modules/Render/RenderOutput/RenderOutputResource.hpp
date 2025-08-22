#pragma once

#include <vulkan/vulkan.hpp>

namespace Raindrop::Render{
    struct RenderOutputResource{
        vk::Semaphore imageAvailableSemaphore;
        vk::Fence imageAvailableFence;

        vk::Semaphore renderFinishedSemaphore;
        vk::Fence renderFinishedFence;

        vk::Image image;
        
        bool available;
    };
}