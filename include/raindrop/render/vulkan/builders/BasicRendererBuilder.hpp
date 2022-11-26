#pragma once

#include "../SwapChain.hpp"
#include "../enum.hpp"

namespace rnd::render::vulkan::renderer{
	class BasicRendererBuilder{
		friend class BasicRenderer;
		public:
			BasicRendererBuilder(){
				clearColor.float32[0] = 0.f;
				clearColor.float32[1] = 0.f;
				clearColor.float32[2] = 0.f;
				clearColor.float32[3] = 0.f;
				
				depthStencilClearValue.depth = 1.f;
				depthStencilClearValue.stencil = 0.f;
			}

			~BasicRendererBuilder() = default;

			void setExtent(VkExtent2D extent){
				this->extent = extent;
			}

			void setBufferCount(uint32_t count){
				bufferCount = 2;
			}

			void setRefreshMode(SwapChainRefreshMode refreshMode){
				this->refreshMode = refreshMode;
			}

			void setDevice(LogicalDevice* device){
				this->device = device;
			}

		private:
			LogicalDevice* device = nullptr;
			VkExtent2D extent = {1, 1};
			SwapChainRefreshMode refreshMode = SwapChainRefreshMode::VSync;
			VkClearColorValue clearColor;
			VkClearDepthStencilValue depthStencilClearValue;
			uint32_t bufferCount = 2;
	};
}