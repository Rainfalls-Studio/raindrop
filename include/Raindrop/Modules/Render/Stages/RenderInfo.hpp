#pragma once

#include <vulkan/vulkan.hpp>

namespace Raindrop::Render{
    struct RenderInfo{
        vk::Semaphore imageAvailable;
        vk::PipelineStageFlags ImageAvailableWaitStageFlags;
        uint32_t currentFrame;

        bool available;

        vk::Semaphore renderFinishedSignal;
        vk::Fence renderFinishedFence;
    };
}