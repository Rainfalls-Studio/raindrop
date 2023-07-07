#ifndef __RAINDROP_GRAPHICS_DESCRIPTOR_POOL_HPP__
#define __RAINDROP_GRAPHICS_DESCRIPTOR_POOL_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class DescriptorPool{
		public:
			DescriptorPool(GraphicsContext& context, VkDescriptorPoolCreateInfo& info);
			~DescriptorPool();

			VkDescriptorPool pool() const;

		private:
			GraphicsContext& _context;
			VkDescriptorPool _pool = VK_NULL_HANDLE;
	};
}

#endif