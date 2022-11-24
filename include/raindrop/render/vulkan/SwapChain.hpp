/**
 * @brief this file contain the vulkan memory buffer
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 */

#ifndef __RAINDROP_RENDER_VULKAN_SWAPCHAIN_HPP__
#define __RAINDROP_RENDER_VULKAN_SWAPCHAIN_HPP__
#include "builders/SwapChainBuilder.hpp"
#include "Framebuffer.hpp"
#include <memory>

namespace rnd::render::vulkan{
	class SwapChain{
		public:
			~SwapChain();
			SwapChain(SwapChainBuilder &builder){
				initialize(builder);
			}
			SwapChain() = default;

			SwapChain(std::shared_ptr<SwapChain> previous);
			void initialize(SwapChainBuilder &builder);

			SwapChain(const SwapChain &) = delete;
			SwapChain& operator=(const SwapChain &) = delete;

			VkFormat findDepthFormat();
			VkFramebuffer getFrameBuffer(uint8_t index);
			VkImageView getImageView(uint8_t index);
			VkRenderPass getRenderPass();
			uint8_t getImageCount();
			VkFormat getImageFormat();
			VkExtent2D getExtent();
			uint8_t getFrameInFlightCount();

			VkResult acquireNextImage(uint32_t *imageIndex);
			VkResult submitCommandBuffer(VkCommandBuffer* buffers, uint32_t *imageIndex);

			bool compareFormats(const SwapChain& swapChain);

			SwapChainRefreshMode getRefreshMode(); 
			VkImage getCurrentImage();
		private:
			void create();
			void createSwapchain();
			void createFramebuffers();
			void createSyncObjects();
			void createRenderPass();

			LogicalDevice* device = nullptr;

			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);


			VkSwapchainKHR swapchain = VK_NULL_HANDLE;
			std::shared_ptr<SwapChain> oldSwapChain = nullptr;
			VkExtent2D windowExtent;
			std::vector<Framebuffer> framebuffers;

			std::vector<VkImage> images;
			std::vector<VkSemaphore> imageAvailableSemaphores;
			std::vector<VkSemaphore> renderFinishedSemaphores;
			std::vector<VkFence> inFlightFences;
			std::vector<VkFence> imagesInFlight;
			size_t currentFrame = 0;
			size_t framesInFlight;

			VkRenderPass renderPass = VK_NULL_HANDLE;

			SwapChainRefreshMode refreshType = SwapChainRefreshMode::FIFO_MODE;
			VkSurfaceFormatKHR wantedSurfaceFormat = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

			VkFormat imageFormat;
			VkFormat depthFormat;
			VkExtent2D extent;
	};
}

#endif