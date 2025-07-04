#pragma once

#include "../PipelineStage.hpp"
#include "translation.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkPipelineStageFlagBits toVulkan(PipelineStageBits&& bit);

    template<>
    VkPipelineStageFlags toVulkan(PipelineStage&& bit);
}