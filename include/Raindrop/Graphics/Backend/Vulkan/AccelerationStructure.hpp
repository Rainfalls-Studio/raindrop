#pragma once

#include "../AccelerationStructure.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class AccelerationStructure : public Backend::AccelerationStructure{
        public:
            AccelerationStructure(Context& context, const Description& description);
            virtual ~AccelerationStructure() override;

            VkAccelerationStructureKHR get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkAccelerationStructureKHR _AccelerationStructure;
    };
}