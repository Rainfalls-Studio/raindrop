#pragma once

#include "../CommandPool.hpp"
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkCommandPoolCreateFlagBits toVulkan(Backend::CommandPool::FlagBits&& bit){
        using enum Backend::CommandPool::FlagBits;
        switch (bit){
            case NONE: return VkCommandPoolCreateFlagBits(0);
            case TRANSIENT: return VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            case RESET: return VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            default: break;
        }
        throw std::runtime_error("Undefined command pool flag");
    }

    RAINDROP_TO_VK_FLAG(VkCommandPoolCreateFlags, VkCommandPoolCreateFlagBits, Backend::CommandPool::Flags)

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