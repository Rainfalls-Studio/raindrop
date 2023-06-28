#ifndef __RAINDROP_GRAPHICS_GRAPHICS_COMMAND_POOL_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_COMMAND_POOL_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsCommandPool{
		public:
			GraphicsCommandPool(GraphicsContext& context);
			~GraphicsCommandPool();

			VkCommandPool primary() const;
			VkCommandPool singleTime() const;

			VkCommandBuffer beginSingleTime();
			void endSingleTime(VkCommandBuffer commandBuffer);

		private:
			GraphicsContext& _context;
			VkCommandPool _primaryCommandPool = VK_NULL_HANDLE;
			VkCommandPool _singleTimeCommandPool = VK_NULL_HANDLE;
	};
}

#endif