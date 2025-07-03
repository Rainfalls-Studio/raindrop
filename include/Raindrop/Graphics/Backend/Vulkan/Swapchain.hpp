#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>
#include "../Swapchain.hpp"
#include "Semaphore.hpp"
#include "translation.hpp"
#include "Surface.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	template<>
	VkPresentModeKHR toVulkan(Backend::Swapchain::PresentMode &&value){
		using enum Backend::Swapchain::PresentMode;
		switch (value){
			case IMMEDIATE: return VK_PRESENT_MODE_IMMEDIATE_KHR;
			case MAILBOX: return VK_PRESENT_MODE_MAILBOX_KHR;
			case FIFO: return VK_PRESENT_MODE_FIFO_KHR;
		}
		throw std::runtime_error("Unknown present mode");
	}

	struct Context;
	class Swapchain : public Backend::Swapchain{
		public:
			Swapchain(Context& context, const Description& description);
			virtual ~Swapchain() override;

            virtual uint32_t getImageCount() const override;

            virtual Format getFormat() const override;

            virtual Extent2D getExtent() const override;

            virtual const std::vector<std::shared_ptr<Image>>& getImages() const override;

            virtual void rebuild() override;

			virtual uint32_t acquireNextImage(
				uint32_t& imageIndex,
                std::shared_ptr<Semaphore> signalSemaphore = nullptr,
                std::shared_ptr<Fence> signalFence = nullptr,
                uint32_t timeout = UINT32_MAX
            ) override;

			virtual void present(
                std::shared_ptr<Queue> queue,
                uint32_t imageIndex,
                const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores = {}
            ) override;

            virtual void setPresentMode(PresentMode mode) override;

			VkSwapchainKHR get() const noexcept;

		private:
			Context& _context;
			std::shared_ptr<Surface> _surface;

			VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
			
			uint32_t _frameCount = 0;
			VkExtent2D _extent = {0, 0};
			VkPresentModeKHR _presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			VkSurfaceFormatKHR _surfaceFormat = {VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
			std::vector<VkImage> _images;

			// Not released because the swapchain get continiously rebuilt
			struct{
				uint32_t frameCount;
				VkExtent2D extent;
				VkPresentModeKHR presentMode;
				VkSurfaceFormatKHR surfaceFormat;
			} _buildInfo;

			
			VkSurfaceFormatKHR findSurfaceFormat();
			VkPresentModeKHR findPresentMode();
			VkExtent2D findExtent();
			uint32_t findFrameCount();
	};
}