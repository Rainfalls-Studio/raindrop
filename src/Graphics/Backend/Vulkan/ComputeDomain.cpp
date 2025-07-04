#include "Raindrop/Graphics/Backend/Vulkan/ComputeDomain.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    ComputeDomain::ComputeDomain(VkCommandBuffer commandBuffer) : _commandBuffer{commandBuffer}{}
    ComputeDomain::~ComputeDomain(){}
            
    void ComputeDomain::dispatch(){

    }

    void ComputeDomain::distatchIndirect(){

    }
}