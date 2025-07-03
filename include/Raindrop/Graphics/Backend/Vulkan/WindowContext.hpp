#pragma once

#include "Context.hpp"
#include "Raindrop/Window/Window.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	class WindowContext{
		public:
			struct FrameData{
				VkFramebuffer framebuffer = VK_NULL_HANDLE;
				VkSemaphore imageAvailable = VK_NULL_HANDLE;
				VkSemaphore imageFinished = VK_NULL_HANDLE;
				VkFence inFlightFence = VK_NULL_HANDLE;
				VkFence imageInFlight = VK_NULL_HANDLE;
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};
			
			WindowContext(Context& context, std::shared_ptr<Window::Window> window);
			~WindowContext();

			void rebuildSwapchain();
			WindowContext& wantExtent(VkExtent2D extent);
			WindowContext& wantFrameCount(uint32_t count);
			WindowContext& wantPresentMode(VkPresentModeKHR presentMode);
			WindowContext& wantSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);

			VkCommandBuffer beginCommandBuffer();
			void submitCommandBuffer();

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			uint32_t getFrameCount() const;
			uint32_t getCurrentFrame() const;
			uint32_t getNextFrame() const;

			const FrameData& getFrameData(uint32_t frame) const;

		private:
			Context& _context;

			std::weak_ptr<Window::Window> _window;

			VkSurfaceKHR _surface = VK_NULL_HANDLE;
			VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
			VkRenderPass _renderPass = VK_NULL_HANDLE;
			VkCommandPool _commandPool = VK_NULL_HANDLE;
			std::vector<FrameData> _frames;
			std::vector<VkCommandBuffer> _commandBuffers;
			VkClearColorValue _clearColor;

			uint32_t _currentFrame;
			uint32_t _frameCount;
			uint32_t _nextFrame;

			VkExtent2D _extent;
			VkPresentModeKHR _presentMode;
			VkSurfaceFormatKHR _surfaceFormat;

			struct{
				uint32_t frameCount;
				VkExtent2D extent;
				VkPresentModeKHR presentMode;
				VkSurfaceFormatKHR surfaceFormat;
			} _buildInfo;

			struct{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			} _support;

			VkResult aquireNextImage();

			void querySupport();
			void querySupportCapabilities();
			void querySupportFormats();
			void querySupportPresentModes();

			uint32_t findFrameCount();
			VkExtent2D findExtent();
			VkPresentModeKHR findPresentMode();
			VkSurfaceFormatKHR findSurfaceFormat();
			
			void getSwapchainFrames();
			void resetSwapchainFrames();
        	void getSwapchainImages();
        	void createImageViews();
        	void createFramebuffers();
        	void createSyncObjects();
        	void allocateCommandBuffers();
			
			void createSurface();
			void createRenderPass();
			void createCommandPool();
	};
}