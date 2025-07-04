#pragma once

#include "../ComputeDomain.hpp"
#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{

    class ComputeDomain : public Backend::ComputeDomain{
        public: 
            ComputeDomain(VkCommandBuffer commandBuffer);
            virtual ~ComputeDomain() override;
            
            virtual void dispatch() override;
            virtual void distatchIndirect() override;
        
        private:
            VkCommandBuffer _commandBuffer;
    };
}