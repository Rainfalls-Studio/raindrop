#pragma once

#include <vk_mem_alloc.h>
#include "../Buffer.hpp"
#include "Device.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
	class Buffer : public Backend::Buffer{
		public:
			Buffer(Context& context, VkBuffer buffer);

            virtual void* map(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void unmap() override;

            virtual void flush(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void invalidate(Size size = SIZE_WHOLE, Size offset = 0) override;

            virtual void* getHandle() const override;

		private:
			Context& _context;
			VkBuffer _buffer;
	};
}