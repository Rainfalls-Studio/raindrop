#ifndef __RAINDROP_GRAPHICS_SWAPCHAIN_HPP__
#define __RAINDROP_GRAPHICS_SWAPCHAIN_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Swapchain{
		public:
			struct Frame{
				VkFramebuffer framebuffer = VK_NULL_HANDLE;
				VkSemaphore imageAvailable = VK_NULL_HANDLE;
				VkSemaphore imageFinished = VK_NULL_HANDLE;
				VkFence inFlightFence = VK_NULL_HANDLE;
				VkFence imageInFlight = VK_NULL_HANDLE;
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};

			Swapchain(GraphicsContext& context);
			~Swapchain();

			void rebuildSwapchain();

			void setExtent(VkExtent2D extent);
			void setFrameCount(uint32_t count);
			void setPresentMode(VkPresentModeKHR presentMode);
			void setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);

			VkResult acquireNextImage();
			VkResult submitCommandBuffer(VkCommandBuffer* buffers);

			uint32_t frameCount() const;
			uint32_t currentFrame() const;
			VkRenderPass renderPass() const;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(VkClearColorValue color);

			Frame& getFrameData(uint32_t id);
			Frame& getCurrentFrameData();
			std::vector<Frame>& getFramesData();

		private:
			struct SwapchainData{
				GraphicsContext& _context;
				VkSwapchainKHR _swapchain = VK_NULL_HANDLE;			
				std::vector<Frame> _frames;

				~SwapchainData();
				SwapchainData(GraphicsContext& _context);
			};

			GraphicsContext& _context;
			SwapchainSupport _swapchainSupport;

			std::unique_ptr<SwapchainData> _swapchain = nullptr;
			std::unique_ptr<SwapchainData> _oldSwapchain = nullptr;

			VkRenderPass _renderPass = VK_NULL_HANDLE;
			uint32_t _currentFrame = 0;
			VkFormat _imageFormat;

			uint32_t _frameCount = 0;
			VkExtent2D _extent;
			VkPresentModeKHR _presentMode;
			VkSurfaceFormatKHR _surfaceFormat;

			uint32_t _wantedFrameCount = 0;
			VkExtent2D _wantedExtent;
			VkPresentModeKHR _wantedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			VkSurfaceFormatKHR _wantedSurfaceFormat = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

			VkClearColorValue _clearColor = {0.f, 0.f, 0.f, 0.f};

			void findSurfaceFormat();
			void findPresentMode();
			void findExtent();
			void findFrameCount();

			void createRenderPass();
			void createImageViews();
			void getSwapchainImages();
			void createFramebuffers();
			void createSyncObjects();
			
	};
}

#endif