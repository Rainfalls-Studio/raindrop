#pragma once

#include "vulkan/vulkan_core.h"
namespace Raindrop::Graphics::Backend::Vulkan{
    template<typename T, typename U>
    T toVulkan(U&& value);

    template<typename T, typename U>
    T toRaindrop(U&& value);
}

#define RAINDROP_TO_VK_FLAG(vkFlag, VkBit, RNDFlag) \
    template<> \
    vkFlag toVulkan(RNDFlag&& value){ \
        using Type = RNDFlag::UnderlyingType; \
        vkFlag flag = vkFlag(0); \
        for (Type bit = 0; bit < static_cast<Type>(RNDFlag::__SIZE__); bit<<=1){ \
            if (value & (1 << bit)){ \
                flag |= static_cast<vkFlag>(toVulkan<VkBit>(static_cast<typename RNDFlag::Bit>(bit))); \
            } \
        } \
        return flag;\
	}
