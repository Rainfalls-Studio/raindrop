#include "Raindrop/Graphics/Backend/Vulkan/Swapchain.hpp"
#include "Raindrop/Graphics/Backend/API.hpp"
#include "Raindrop/Graphics/Backend/Semaphore.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Semaphore.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Fence.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Surface.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "spdlog/spdlog.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

namespace Raindrop::Graphics::Backend::Vulkan{

	Swapchain::Swapchain(Context& context, const std::shared_ptr<Surface>& surface) :  _context{context}{
		// Ensure the surface is from the same API
		if (surface->getAPI() != API::VULKAN){
			throw std::runtime_error("The surface is not from the Vulkan API");
		}

		_surface = surface;
	}

	Swapchain::~Swapchain(){
		SPDLOG_LOGGER_TRACE(_context.logger, "Destroying swapchain...");
		
		if (_swapchain){
			vkDestroySwapchainKHR(_context.device, _swapchain, nullptr);
			_swapchain = VK_NULL_HANDLE;
		}
	}

	VkSwapchainKHR Swapchain::get() const noexcept{
		return _swapchain;
	}

	VkSurfaceFormatKHR Swapchain::findSurfaceFormat(const VkSurfaceFormatKHR& target){

		/**
		 * I thought it would be better to find the most suitable
		 * format instead of fallbacking on a random format
		 * if we fail to find the requested format
		 */
		
		const auto& support = _surface->getFormats();

		VkSurfaceFormatKHR bestFormat = support[0];
		uint32_t bestFormatScore = 0;

		for (const auto& format : support){
			uint32_t score = 0;

			score += int(format.colorSpace == target.colorSpace);
			score += int(format.format == target.format);

			if (score > bestFormatScore){
				bestFormat = format;
				bestFormatScore = score;
			}
		}

		return bestFormat;
	}

	VkPresentModeKHR Swapchain::findPresentMode(VkPresentModeKHR target){
		const auto& support = _surface->getPresentModes();

		for (const auto& mode : support){
			if (mode == target){
				return mode;
			}
		}

		return support[0];
	}

	VkExtent2D Swapchain::findExtent(const VkExtent2D& target){
		auto capabilities = _surface->getCapabilities();

		VkExtent2D extent;

		if (capabilities.currentExtent.width != UINT32_MAX){
			return capabilities.currentExtent;
		}

		extent.width = std::clamp(target.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(target.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	std::uint32_t Swapchain::findImageCount(uint32_t target){
		const auto& capabilities = _surface->getCapabilities();
		return std::clamp(target, capabilities.minImageCount, capabilities.maxImageCount == 0 ? UINT32_MAX : capabilities.maxImageCount);
	}

	void Swapchain::rebuild(const Description& description){
		auto& device = _context.device;

		auto oldSurfaceFormat = _surfaceFormat;
		
		_imageCount = findImageCount(description.imageCount);
		_extent = findExtent(toVulkan<VkExtent2D>(description.extent));
		_presentMode = findPresentMode(toVulkan<VkPresentModeKHR>(description.presentMode));
		_surfaceFormat = findSurfaceFormat({toVulkan<VkFormat>(description.format), VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});

		_context.logger->info("Rebuilding swapchain...");
		
		// maybe use vkQueueWaitIdle(device.presentQueue.queue);
		// device->waitIdle();
		vkDeviceWaitIdle(device);

		VkSwapchainKHR oldSwapchain = _swapchain;

		const bool hasFormatChanged = oldSurfaceFormat.format != _surfaceFormat.format;
		const bool hasColorSpaceChanged = oldSurfaceFormat.colorSpace != _surfaceFormat.colorSpace;

		if (hasColorSpaceChanged || hasFormatChanged){
			SPDLOG_LOGGER_ERROR(_context.logger,
				"The swapchain surface format and / or color space has changed\n"
				"\t - Format (before/after) : {} / {}\n"
				"\t - Colorspace (before/after) : {} / {}",
				string_VkFormat(_surfaceFormat.format), string_VkFormat(oldSurfaceFormat.format),
				string_VkColorSpaceKHR(_surfaceFormat.colorSpace), string_VkColorSpaceKHR(oldSurfaceFormat.colorSpace)
			);
			throw std::runtime_error("The surface format and/or color space has changed");
		}

		auto capabilities = _surface->getCapabilities();
		
		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.surface = _surface->get();
		info.oldSwapchain = oldSwapchain;
		info.preTransform = capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = _presentMode;
		info.clipped = VK_TRUE;

		uint32_t queueFamilyIndices[] = {0, 1
			// device->graphicsQueue.familyIndex,
			// device->graphicsQueue.familyIndex,
		};

		if (queueFamilyIndices[0] != queueFamilyIndices[1]){
			info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			info.queueFamilyIndexCount = 2;
			info.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
		}
		
		info.minImageCount = _imageCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (vkCreateSwapchainKHR(device, &info, nullptr, &_swapchain) != VK_SUCCESS){
			throw std::runtime_error("Failed to create swapchain");
		}

		getSwapchainImages();

		SPDLOG_LOGGER_TRACE(_context.logger, "The vulkan swapchain has been rebuilt without any critical error");
	}

	void Swapchain::getSwapchainImages(){
		auto& device = _context.device;

		uint32_t imageCount = 0;
		if (vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, nullptr) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}

		if (imageCount != _imageCount){
			_context.logger->info("The number of swapchain frame changed from {} to {}", _imageCount, imageCount);
		}

		_imageCount = imageCount;

		_images.resize(_imageCount);
		if (vkGetSwapchainImagesKHR(device, _swapchain, &_imageCount, _images.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}
	}

	uint32_t Swapchain::acquireNextImage(std::shared_ptr<Backend::Semaphore> signalSemaphore, std::shared_ptr<Backend::Fence> signalFence, uint32_t timeout){
		assert(signalSemaphore ? signalSemaphore->getAPI() == API::VULKAN : true && "The semaphore must be from the Vulkan API");
		assert(signalFence ? signalFence->getAPI() == API::VULKAN : true && "The fence must be from the Vulkan API");

		VkSemaphore semaphore = signalSemaphore ? std::static_pointer_cast<Semaphore>(signalSemaphore)->get() : VK_NULL_HANDLE;
		VkFence fence = signalFence ? std::static_pointer_cast<Fence>(signalFence)->get() : VK_NULL_HANDLE;

		auto& device = _context.device;

		uint32_t nextFrame = 0;

		VkResult result = vkAcquireNextImageKHR(
			device,
			_swapchain,
			timeout,
			semaphore,
			fence,
			&nextFrame
		);

		if (result == VK_SUBOPTIMAL_KHR) _suboptimal = true;
		if (result == VK_ERROR_OUT_OF_DATE_KHR) _outOfDate = true;
		// TODO: check errors

		return nextFrame;
	}

	void Swapchain::present(std::shared_ptr<Backend::Queue> queue, uint32_t imageIndex, std::shared_ptr<Backend::Semaphore> waitSemaphore){
		assert(queue && "A valid queue must be provided");
		assert(queue->getAPI() == API::VULKAN && "The queue must be from the Vulkan API");
		// assert(queue->getCapabilities() & Queue::Capabilities::PRESENT && "The queue must support presenting images");
		assert(imageIndex < _imageCount && imageIndex > 0 && "Invalid image index"); 
		assert(waitSemaphore ? waitSemaphore->getAPI() == API::VULKAN : true && "The semaphore must be from the Vulkan API");

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		VkSemaphore semaphore = waitSemaphore ? std::static_pointer_cast<Semaphore>(waitSemaphore)->get() : VK_NULL_HANDLE;

		if (semaphore){
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &semaphore;
		}

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &_swapchain;
		presentInfo.pImageIndices = &imageIndex;

		// TODO
		// VkResult result = vkQueuePresentKHR(device->presentQueue.queue, &presentInfo);

		VkResult result;

		if (result == VK_SUBOPTIMAL_KHR) _suboptimal = true;
		if (result == VK_ERROR_OUT_OF_DATE_KHR) _outOfDate = true;
	}


	bool Swapchain::isSuboptimal() const noexcept{
		return _suboptimal;
	}

	bool Swapchain::isOutOfDate() const noexcept{
		return _outOfDate;
	}

	void* Swapchain::getHandle() const noexcept{
		return static_cast<void*>(_swapchain);
	}

	API Swapchain::getAPI() const noexcept{
		return API::VULKAN;
	}
}