#pragma once

#include <stdexcept>
#include <vulkan/vulkan.h>
#include "../Buffer.hpp"
#include "Context.hpp"
#include "translation.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{

	template<>
	VkBufferUsageFlagBits toVulkan(Backend::Buffer::UsageBits&& bit);

	template<>
	VkBufferUsageFlags toVulkan(Backend::Buffer::Usage&& bit);

	class Buffer : public Backend::Buffer{
		public:
			Buffer(Context& context, const Description& buffer);
			virtual ~Buffer() override;

            virtual void* map(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void unmap() override;

            virtual void flush(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void invalidate(Size size = SIZE_WHOLE, Size offset = 0) override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

		private:
			Context& _context;
			VkBuffer _buffer = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
	};
}