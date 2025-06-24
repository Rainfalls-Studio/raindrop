#pragma once

#include <VkBootstrap.h>
#include <spdlog/spdlog.h>
#include <vk_mem_alloc.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    struct Context{
        std::shared_ptr<spdlog::logger> logger;
        vkb::Instance instance;
        vkb::PhysicalDevice physicalDevice;
        vkb::Device device;
        VmaAllocator allocator;
    };
}