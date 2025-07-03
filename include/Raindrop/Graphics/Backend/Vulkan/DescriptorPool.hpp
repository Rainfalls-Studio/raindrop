#pragma once

#include "../DescriptorPool.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class DescriptorPool : public Backend::DescriptorPool{
        public:
            DescriptorPool(Context& context, const Description& description);
            virtual ~DescriptorPool() override;

            VkDescriptorPool get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkDescriptorPool _descriptorPool;
    };
}