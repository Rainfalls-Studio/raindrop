#include "../Extent.hpp"
#include <vulkan/vulkan.h>
#include "translation.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkExtent2D toVulkan(Extent2D&& extent);
    
    template<>
    VkExtent3D toVulkan(Extent3D&& extent);
}
