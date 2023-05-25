#ifndef __RAINDROP_GRAPHICS_SWAPCHAIN_HPP__
#define __RAINDROP_GRAPHICS_SWAPCHAIN_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>

namespace Raindrop::Graphics{
	class Swapchain{
		public:
			Swapchain(const std::shared_ptr<Device>& device, VkSurfaceKHR surface, VkExtent2D extent, VkAllocationCallbacks* allocationCallbacks = nullptr);
			~Swapchain();

			void rebuildSwapchain();

			void setExtent(VkExtent2D extent);
			void setFrameCount(uint32_t count);
			void setPresentMode(VkPresentModeKHR presentMode);
			void setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);

			VkResult acquireNextImage();
			VkResult submitCommandBuffer(VkCommandBuffer* buffers);

			void setGraphicsQueue(VkQueue queue);
			void setPresentQueue(VkQueue queue);

			uint32_t frameCount() const;
			uint32_t currentFrame() const;
			VkRenderPass renderPass() const;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(VkClearColorValue color);

		private:
			VkAllocationCallbacks* _allocationCallbacks;
			VkSurfaceKHR _surface;
			std::shared_ptr<Device> _device;
			PhysicalDevice::SwapchainSupport _swapchainSupport;

			VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
			VkSwapchainKHR _oldSwapchain = VK_NULL_HANDLE;

			struct Frame{
				VkFramebuffer framebuffer;
				VkSemaphore imageAvailable;
				VkSemaphore imageFinished;
				VkFence inFlightFence;
				VkFence imageInFlight;
				VkImage image;
				VkImageView imageView;
			};

			VkRenderPass _renderPass = VK_NULL_HANDLE;
			std::vector<Frame> _frames;
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

			VkQueue _graphicsQueue = VK_NULL_HANDLE;
			VkQueue _presentQueue = VK_NULL_HANDLE;

			VkClearColorValue _clearColor = {0.f, 0.f, 0.f, 0.f};

			void findSurfaceFormat();
			void findPresentMode();
			void findExtent();
			void findFrameCount();
			
			void updateFrameArray();
			void destroyFrame(Frame& frame);
			void destroyFrames();
			void createRenderPass();
			void createImageViews();
			void getSwapchainImages();
			void createFramebuffers();
			void createSyncObjects();
			
	};
}

#endif