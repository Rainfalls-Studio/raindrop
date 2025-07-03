#include "Raindrop/Graphics/Backend/Vulkan/Swapchain.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Surface.hpp"
#include "spdlog/spdlog.h"

#include <stdexcept>
#include <vulkan/vk_enum_string_helper.h>

#include <iostream>
#include <vulkan/vulkan_core.h>

namespace Raindrop::Graphics::Backend::Vulkan{

	Swapchain::Swapchain(Context& context, const Description& description) :  _context{context}{
		_surface = description.surface;

		_frameCount = findFrameCount();
		_extent = findExtent();
		_presentMode = findPresentMode();
		_surfaceFormat = findSurfaceFormat();
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

	VkSurfaceFormatKHR Swapchain::findSurfaceFormat(){

		/**
		 * I thought it would be better to find the most suitable
		 * format instead of fallbacking on a random format
		 * if we fail to find the requested format
		 */
		
		const auto& wanted = _buildInfo.surfaceFormat;
		// const auto& support = _surface->();

		VkSurfaceFormatKHR bestFormat = _support.formats[0];
		uint32_t bestFormatScore = 0;

		for (const auto& format : _support.formats){
			uint32_t score = 0;

			score += int(format.colorSpace == wanted.colorSpace);
			score += int(format.format == wanted.format);

			if (score > bestFormatScore){
				bestFormat = format;
				bestFormatScore = score;
			}
		}

		return bestFormat;
	}

	VkPresentModeKHR Swapchain::findPresentMode(){
		const auto& wanted = _buildInfo.presentMode;

		for (const auto& mode : _support.presentModes){
			if (mode == wanted){
				return mode;
			}
		}

		return _support.presentModes[0];
	}

	VkExtent2D Swapchain::findExtent(){
		const auto& wanted = _buildInfo.extent;
		auto& capabilities = _support.capabilities;

		VkExtent2D extent;

		if (capabilities.currentExtent.width != UINT32_MAX){
			return capabilities.currentExtent;
		}

		extent.width = std::clamp(wanted.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(wanted.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	std::uint32_t Swapchain::findFrameCount(){
		const auto& capabilities = _support.capabilities;
		return std::clamp(_buildInfo.frameCount, capabilities.minImageCount, capabilities.maxImageCount == 0 ? UINT32_MAX : capabilities.maxImageCount);
	}


	Swapchain& Swapchain::wantExtent(VkExtent2D extent){
		_buildInfo.extent = extent;
		return *this;
	}

	Swapchain& Swapchain::wantFrameCount(uint32_t count){
		_buildInfo.frameCount = count;
		return *this;
	}

	Swapchain& Swapchain::wantPresentMode(VkPresentModeKHR presentMode){
		_buildInfo.presentMode = presentMode;
		return *this;
	}

	Swapchain& Swapchain::wantSurfaceFormat(VkSurfaceFormatKHR surfaceFormat){
		_buildInfo.surfaceFormat = surfaceFormat;
		return *this;
	}

	void Swapchain::rebuildSwapchain(){
		auto& device = _context.device;

		_context.logger->info("Rebuilding swapchain...");
		
		// maye vkQueueWaitIdle(device.presentQueue.queue);
		// device->waitIdle();
		vkDeviceWaitIdle(device);
		
		std::swap(_swapchain, _oldSwapchain);
		_swapchain = std::make_unique<SwapchainData>(_context);

		querySupport();

		VkSurfaceFormatKHR surfaceFormat = findSurfaceFormat();
		const bool hasFormatChanged = surfaceFormat.format != _surfaceFormat.format;
		const bool hasColorSpaceChanged = surfaceFormat.colorSpace != _surfaceFormat.colorSpace;

		if (hasColorSpaceChanged || hasFormatChanged){
			_context.logger->error(
				"The swapchain surface format and / or color space has changed\n"
				"\t - Format (before/after) : {} / {}\n"
				"\t - Colorspace (before/after) : {} / {}",
				string_VkFormat(_surfaceFormat.format), string_VkFormat(surfaceFormat.format),
				string_VkColorSpaceKHR(_surfaceFormat.colorSpace), string_VkColorSpaceKHR(surfaceFormat.colorSpace)
			);
			throw std::runtime_error("The surface format and/or color space has changed");
		}

		_frameCount = findFrameCount();
		_extent = findExtent();
		_presentMode = findPresentMode();
		
		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.surface = _surface;
		info.oldSwapchain = _oldSwapchain != nullptr ? _oldSwapchain->swapchain : VK_NULL_HANDLE;
		info.preTransform = _support.capabilities.currentTransform;
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
		
		info.minImageCount = _frameCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (vkCreateSwapchainKHR(device, &info, nullptr, &_swapchain->swapchain) != VK_SUCCESS){
			throw std::runtime_error("Failed to create swapchain");
		}
		_oldSwapchain.reset();

		getSwapchainImages();
		createImageViews();
		createFramebuffers();

		createSyncObjects();

		_currentFrame = 0;
		_context.logger->info("The vulkan swapchain has been rebuilt without any critical error");
	}

	void Swapchain::getSwapchainImages(){
		auto& device = _context.device;

		uint32_t imageCount = 0;
		if (vkGetSwapchainImagesKHR(device, _swapchain->swapchain, &imageCount, nullptr) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}

		if (imageCount != _frameCount){
			_context.logger->info("The number of swapchain frame changed from {} to {}", _frameCount, imageCount);
		}

		_frameCount = imageCount;

		std::vector<VkImage> images(_frameCount);
		if (vkGetSwapchainImagesKHR(device, _swapchain->swapchain, &_frameCount, images.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}

		// forcefully reload everything to ensure proper destroy
		_swapchain->frames.clear();
		_swapchain->frames.resize(_frameCount);

		for (uint32_t i=0; i<_frameCount; i++){
			_swapchain->frames[i].image = images[i];
		}
	}

	VkResult Swapchain::acquireNextImage(){
		auto& device = _context.device;

		if (vkWaitForFences(device, 1, &_swapchain->frames[_currentFrame].inFlightFence, VK_TRUE, UINT64_MAX) != VK_SUCCESS){
			throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
		}

		VkResult result = vkAcquireNextImageKHR(device, _swapchain->swapchain, UINT64_MAX, _swapchain->frames[_currentFrame].imageAvailable, VK_NULL_HANDLE, &_nextFrame);
		return result;
	}

	VkResult Swapchain::submitCommandBuffer(std::vector<VkCommandBuffer> buffers){
		auto& device = _context.device;
		
		if (_swapchain->frames[_nextFrame].imageInFlight != VK_NULL_HANDLE){
			if (vkWaitForFences(device, 1, &_swapchain->frames[_nextFrame].imageInFlight, VK_TRUE, UINT64_MAX) != VK_SUCCESS){
				throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
			}
		}
		
		_swapchain->frames[_nextFrame].imageInFlight = _swapchain->frames[_currentFrame].inFlightFence;


		VkSemaphore waitSemaphores[] = {_swapchain->frames[_currentFrame].imageAvailable};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores[] = {_swapchain->frames[_currentFrame].imageFinished};

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());
		submitInfo.pCommandBuffers = buffers.data();

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkResetFences(device, 1, &_swapchain->frames[_currentFrame].inFlightFence) != VK_SUCCESS){
			throw std::runtime_error("Failed to reset swapchain attachment in flight fence");
		}

		// if (vkQueueSubmit(device->graphicsQueue.queue, 1, &submitInfo, _swapchain->frames[_currentFrame].inFlightFence) != VK_SUCCESS){
		// 	throw std::runtime_error("Faile to submit graphics command buffer");
		// }

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {_swapchain->swapchain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &_nextFrame;

		// VkResult result = vkQueuePresentKHR(device->presentQueue.queue, &presentInfo);

		_currentFrame = (_currentFrame + 1) % _frameCount;
		return VK_ERROR_UNKNOWN;
	}
}