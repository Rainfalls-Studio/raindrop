#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan::Queue{
    struct Queue{
        uint32_t family;
        VkQueue queue;
    };
}