#pragma once

#include "../CommandPool.hpp"
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkCommandPoolCreateFlagBits toVulkan(Backend::CommandPool::FlagBits&& bit);

    template<>
    VkCommandPoolCreateFlags toVulkan(Backend::CommandPool::Flags&& bit);

    class CommandPool : public Backend::CommandPool{
        public:
            CommandPool(Context& context, const Description& description);
            virtual ~CommandPool() override;

            virtual void reset() const override;
            virtual void trim() const override;

            virtual void free(const std::vector<std::shared_ptr<CommandList>>& commands) override;
            virtual std::vector<std::shared_ptr<CommandList>> allocate(uint32_t count, CommandList::Level level) const override;

            VkCommandPool get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkCommandPool _commandPool = VK_NULL_HANDLE;
    };
}