#pragma once

#include "../Queue.hpp"
#include "Context.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    class Queue : public Backend::Queue{
        public:
            Queue(Context& context);
            virtual ~Queue() override;

            virtual Capabilities getCapabilities() const override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

        private:
            Context& _context;
    };
}