#pragma once

#include "../CommandList.hpp"
#include "Context.hpp"
#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class CommandList : public Backend::CommandList{
        public:
            CommandList(Context& context, VkCommandBuffer commandBuffer);
            virtual ~CommandList() override;

            VkCommandBuffer get() const noexcept;

            virtual std::shared_ptr<Domain> getDomain(const std::type_index& type) const override;

            virtual void execute(const std::vector<std::shared_ptr<Backend::CommandList>>& command) override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
    };
}