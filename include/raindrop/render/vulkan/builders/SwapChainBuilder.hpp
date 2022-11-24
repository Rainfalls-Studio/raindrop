#pragma once

#include <vulkan/vulkan.h>
#include "../enum.hpp"
#include "../LogicalDevice.hpp"

namespace rnd::render::vulkan{
	
	class SwapChainBuilder{
		friend class SwapChain;
		public:
			void setRefreshMode(SwapChainRefreshMode refreshMode);
			void setSurfaceFormat(VkSurfaceFormatKHR format);
			void setDevice(LogicalDevice* device);

			// ! cannot be less than 2 buffers !
			void setSwapchainBufferCount(uint8_t count);
			void setExtent(VkExtent2D extent);
		
		private:
			SwapChainRefreshMode refreshMode = SwapChainRefreshMode::FIFO_MODE; // V-Sync
			VkSurfaceFormatKHR format;
			VkExtent2D extent;
			uint8_t frameBufferCount = 2;
			LogicalDevice* device = nullptr;
	};
}