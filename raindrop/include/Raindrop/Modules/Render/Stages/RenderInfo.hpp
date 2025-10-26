#pragma once

#include <vulkan/vulkan.hpp>

namespace Raindrop::Render{
    struct RenderInfo{
        vk::Semaphore imageAvailable;
        vk::PipelineStageFlags imageAvailableWaitStageFlags;
        uint32_t currentFrame;
        uint32_t frameCount;

        bool available;

        vk::Semaphore renderFinishedSemaphore;
        vk::Fence renderFinishedFence;
    };
}