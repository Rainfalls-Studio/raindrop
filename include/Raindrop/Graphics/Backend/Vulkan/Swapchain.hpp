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
	VkPresentModeKHR toVulkan(Backend::Swapchain::PresentMode &&value);

	struct Context;
	class Swapchain : public Backend::Swapchain{
		public:
			Swapchain(Context& context, const std::shared_ptr<Surface>& surface);
			virtual ~Swapchain() override;

            virtual uint32_t getImageCount() const override;

            virtual Format getFormat() const override;

            virtual Extent2D getExtent() const override;

            virtual const std::vector<std::shared_ptr<Image>>& getImages() const override;

            virtual void rebuild(const Description& description) override;

			virtual uint32_t acquireNextImage(
                std::shared_ptr<Backend::Semaphore> signalSemaphore = nullptr,
                std::shared_ptr<Backend::Fence> signalFence = nullptr,
                uint32_t timeout = UINT32_MAX
            ) override;

			virtual void present(
                std::shared_ptr<Backend::Queue> queue,
                uint32_t imageIndex,
                std::shared_ptr<Backend::Semaphore> waitSemaphore = nullptr
            ) override;

            virtual void setPresentMode(PresentMode mode) override;

			VkSwapchainKHR get() const noexcept;

            virtual bool isSuboptimal() const noexcept override;
            virtual bool isOutOfDate() const noexcept override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
			
		private:
			Context& _context;
			std::shared_ptr<Surface> _surface;

			VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
			
			uint32_t _imageCount = 0;
			VkExtent2D _extent = {0, 0};
			VkPresentModeKHR _presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			VkSurfaceFormatKHR _surfaceFormat = {VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
			std::vector<VkImage> _images;
			
			bool _suboptimal;
			bool _outOfDate;

			VkSurfaceFormatKHR findSurfaceFormat(const VkSurfaceFormatKHR& target);
			VkPresentModeKHR findPresentMode(VkPresentModeKHR target);
			VkExtent2D findExtent(const VkExtent2D& target);
			uint32_t findImageCount(uint32_t target);


			void getSwapchainImages();
	};
}