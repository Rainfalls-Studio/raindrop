#pragma once

#include <stdexcept>
#include <vulkan/vulkan.h>
#include "../Format.hpp"
#include "translation.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkFormat toVulkan(Format &&format);
}