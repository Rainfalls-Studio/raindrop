#pragma once

#include "../Queue.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class Queue : public Backend::Queue{
        public:
            Queue(Context& context, VkQueue queue);
            virtual ~Queue() override;

            VkQueue get() const noexcept;

            virtual void waitIdle() const override;
            virtual void submit(const std::vector<Submission>& submissions, std::shared_ptr<Fence> fence = nullptr) override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

        private:
            Context& _context;
            VkQueue _queue;
    };
}