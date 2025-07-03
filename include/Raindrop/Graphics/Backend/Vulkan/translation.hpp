#pragma once

namespace Raindrop::Graphics::Backend::Vulkan{
    template<typename T, typename U>
    T toVulkan(U&& value);
}

#define RAINDROP_TO_VK_FLAG(vkFlag, VkBit, RNDFlag) \
    template<> \
    vkFlag toVulkan(RNDFlag&& usage){ \
        using Type = RNDFlag::UnderlyingType; \
        vkFlag flag = vkFlag(0); \
        for (Type bit = 0; bit < static_cast<Type>(RNDFlag::__SIZE__); bit<<=1){ \
            if (usage & (1 << bit)){ \
                flag |= toVulkan<VkBit>(static_cast<typename RNDFlag::Bit>(bit)); \
            } \
        } \
        return flag;\
	}

