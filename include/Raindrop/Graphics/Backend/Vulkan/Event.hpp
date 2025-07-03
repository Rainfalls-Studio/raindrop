#pragma once

#include "../Event.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Event : public Backend::Event{
        public:
            Event(Context& context, const Description& description);
            virtual ~Event() override;

            VkEvent get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkEvent _event;
    };
}