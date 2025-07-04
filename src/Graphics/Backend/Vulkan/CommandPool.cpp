#include "Raindrop/Graphics/Backend/Vulkan/CommandPool.hpp"
#include "Raindrop/Graphics/Backend/CommandList.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/QueueFamily.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/CommandList.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <stdexcept>

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


    CommandPool::CommandPool(Context& context, const Description& description) : _context{context}{
        assert(description.family && "A valid queue family must be provided");
        assert(description.family->getAPI() == API::VULKAN && "The queue family must be from the Vulkan API");

        VkCommandPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.queueFamilyIndex = std::static_pointer_cast<QueueFamily>(description.family)->getFamilyIndex();
        info.flags = toVulkan<VkCommandPoolCreateFlags>(description.flags);

        if (vkCreateCommandPool(_context.device, &info, nullptr, &_commandPool) != VK_SUCCESS){
            throw std::runtime_error("Failed to create command pool");
        }
    }

    void CommandPool::reset() const{
        if (vkResetCommandPool(_context.device, _commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS){
            throw std::runtime_error("Failed to reset command pool");
        }
    }

    void CommandPool::trim() const{
        vkTrimCommandPool(_context.device, _commandPool, 0);
    }

    void CommandPool::free(const std::vector<std::shared_ptr<Backend::CommandList>>& commands){
        assert(commands.size() > 0 && "No command lists to free");

        std::vector<VkCommandBuffer> commandBuffers(commands.size());
        for (uint32_t i=0; i<commands.size(); i++){
            auto& list = commands[i];
            assert(list && "Each command list must be valid");
            assert(list->getAPI() == API::VULKAN && "The list must be a Vulkan command list");

            commandBuffers[i] = std::static_pointer_cast<CommandList>(list)->get();
        }

        vkFreeCommandBuffers(
            _context.device,
            _commandPool,
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data()
        );
    }

    std::vector<std::shared_ptr<Backend::CommandList>> CommandPool::allocate(uint32_t count, CommandList::Level level) const{
        VkCommandBufferAllocateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandBufferCount = count;
        info.commandPool = _commandPool;
        info.level = toVulkan<VkCommandBufferLevel>(level);

        std::vector<VkCommandBuffer> commandBuffers(count);
        
        if (vkAllocateCommandBuffers(_context.device, &info, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate command buffers");
        }

        std::vector<std::shared_ptr<Backend::CommandList>> lists(count);
        for (uint32_t i=0; i<count; i++){
            lists[i] = std::static_pointer_cast<Backend::CommandList>(std::make_shared<CommandList>(_context, commandBuffers[i]));
        }

        return lists;
    }

    CommandPool::~CommandPool(){
        if (_commandPool){
            vkDestroyCommandPool(_context.device, _commandPool, nullptr);
        }
    }

    VkCommandPool CommandPool::get() const noexcept{
        return _commandPool;
    }

    void* CommandPool::getHandle() const noexcept{
        return static_cast<void*>(_commandPool);
    }

    API CommandPool::getAPI() const noexcept{
        return API::VULKAN;
    }
}