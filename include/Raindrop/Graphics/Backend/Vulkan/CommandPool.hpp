#pragma once

#include "Raindrop/pch.pch"
#include "CommandBuffer.hpp"

namespace Raindrop::Graphics{
	class Context;

	namespace Backend::Vulkan{
		class CommandPool{
			public:
				static std::shared_ptr<CommandPool> create(Context& context);

				CommandPool();
				~CommandPool();

				CommandPool& prepare(Context& context);
				void initialize();
				void release();

				CommandPool& setCreateFlags(const VkCommandPoolCreateFlags& flags);
				CommandPool& setQueueFamily(const std::uint32_t& index);

				const VkCommandPool& get() const noexcept;

				std::vector<CommandBuffer> allocate(const std::uint32_t& count, const VkCommandBufferLevel& level);
				void free(const std::vector<CommandBuffer>& commandBuffers);
				
				void reset(const VkCommandPoolResetFlags& flags);
				void trim(const VkCommandPoolTrimFlags& flags);

			private:
				Context* _context;
				VkCommandPool _pool;

				VkCommandPoolCreateFlags _flags;
				std::uint32_t _familyIndex;
		};
	}
}