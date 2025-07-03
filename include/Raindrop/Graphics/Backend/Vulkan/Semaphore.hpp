#pragma once

#include "../Semaphore.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Semaphore : public Backend::Semaphore{
        public:
            Semaphore(Context& context);
            virtual ~Semaphore() override;

            VkSemaphore get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkSemaphore _semaphore;
    };
}