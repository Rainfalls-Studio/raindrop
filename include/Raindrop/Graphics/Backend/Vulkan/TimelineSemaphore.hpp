#pragma once

#include "../TimelineSemaphore.hpp"
#include "Context.hpp"
#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class TimelineSemaphore : public Backend::TimelineSemaphore{
        public:
            TimelineSemaphore(Context& context, const Description& description);
            virtual ~TimelineSemaphore() override;

            VkSemaphore get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkSemaphore _semaphore;
    };
}