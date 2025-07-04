#include "Raindrop/Graphics/Backend/Vulkan/Extent.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkExtent2D toVulkan(Extent2D&& extent){
        return VkExtent2D{extent.width, extent.height};
    }
    
    template<>
    VkExtent3D toVulkan(Extent3D&& extent){
        return VkExtent3D{extent.width, extent.height, extent.depth};
    }
}