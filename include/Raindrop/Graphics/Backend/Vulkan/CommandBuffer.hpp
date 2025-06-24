#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;
		
	namespace Backend::Vulkan{
		class CommandBuffer{
			public:
				CommandBuffer();
				CommandBuffer(Context& context, VkCommandBuffer commandBuffer);
				~CommandBuffer();

				const VkCommandBuffer& get() const noexcept;

				void beginSingleUse();

				void end();
				void submit(VkQueue queue, VkFence fence = VK_NULL_HANDLE);

			private:
				Context* _context;
				VkCommandBuffer _commandBuffer;
		};
	}
}