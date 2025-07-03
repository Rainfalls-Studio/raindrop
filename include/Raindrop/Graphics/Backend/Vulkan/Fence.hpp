#pragma once

#include "../Fence.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Fence : public Backend::Fence{
        public:
            Fence(Context& context, const Description& description);
            virtual ~Fence() override;

            VkFence get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkFence _fence;
    };
}