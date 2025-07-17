#pragma once

#include <VkBootstrap.h>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <spdlog/logger.h>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Graphics{
    struct Context{
        inline Context(Engine& engine_) : engine{engine_}{}

        Engine& engine;
        std::shared_ptr<spdlog::logger> logger;

        vkb::Instance __instance;
        vkb::PhysicalDevice __physicalDevice;
        vkb::Device __device;

        vk::Instance instance;
        vk::PhysicalDevice physicalDevice;
        vk::Device device;
        
        VmaAllocator allocator;
    };
}