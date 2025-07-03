#pragma once

#include "../DescriptorSetLayout.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class DescriptorSetLayout : public Backend::DescriptorSetLayout{
        public:
            DescriptorSetLayout(Context& context, const Description& description);
            virtual ~DescriptorSetLayout() override;

            VkDescriptorSetLayout get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkDescriptorSetLayout _descriptorSetLayout;
    };
}