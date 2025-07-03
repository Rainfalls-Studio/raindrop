#pragma once

#include "../Framebuffer.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Framebuffer : public Backend::Framebuffer{
        public:
            Framebuffer(Context& context, const Description& description);
            virtual ~Framebuffer() override;

            VkFramebuffer get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkFramebuffer _framebuffer;
    };
}