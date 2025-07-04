#include "Raindrop/Graphics/Backend/Vulkan/WindowContext.hpp"
#include "Raindrop/Window/Flags.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "spdlog/spdlog.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Raindrop::Graphics::Backend::Vulkan{
    WindowContext::WindowContext(Context& context, std::shared_ptr<Window::Window> window) : 
        _context{context},
        _window{window}
    {
        createSurface();
    }

    void WindowContext::createSurface(){
        auto window = _window.lock();

        if (!window){
            throw std::runtime_error("Window is invalid");
        }

        auto surfaceProvider = window->getSurfaceProvider<Window::SurfaceProviders::Vulkan>();
        surfaceProvider->createSurface(_context.instance, nullptr, &_surface);
    }


    void WindowContext::createRenderPass(){
        auto& device = _context.device;
        VkRenderPassCreateInfo info{};

        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        VkAttachmentDescription attachment{};
        attachment.format = _surfaceFormat.format;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;

        info.pAttachments = &attachment;
        info.attachmentCount = 1;

        VkAttachmentReference attachmentRef{};
        attachmentRef.attachment = 0;
        attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassInfo{};
        subpassInfo.colorAttachmentCount = 1;
        subpassInfo.pColorAttachments = &attachmentRef;

        info.subpassCount = 1;
        info.pSubpasses = &subpassInfo;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = {0};
        dependency.srcAccessMask = VK_ACCESS_NONE;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        
        if (vkCreateRenderPass(device, &info, nullptr, &_renderPass) != VK_SUCCESS){
            throw std::runtime_error("Failed to create render pass");
        }
    }

    WindowContext::~WindowContext(){
        auto& device = _context.device;

        resetSwapchainFrames();

        if (_swapchain){
            vkDestroySwapchainKHR(device, _swapchain, nullptr);
        }

        if (_commandPool){
            vkDestroyCommandPool(device, _commandPool, nullptr);
        }

        if (_renderPass){
            vkDestroyRenderPass(device, _renderPass, nullptr);
        }

        if (_surface){
            vkDestroySurfaceKHR(_context.instance, _surface, nullptr);
        }
    }

    void WindowContext::createCommandPool(){
        auto& device = _context.device;

        VkCommandPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // info.queueFamilyIndex = _context.graphics.family;
        // TODO

        if (vkCreateCommandPool(device, &info, nullptr, &_commandPool) != VK_SUCCESS){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create command pool");
            throw std::runtime_error("Failed to create swapchain");
        }
    }

    void WindowContext::rebuildSwapchain(){
        auto& device = _context.device;

        SPDLOG_LOGGER_TRACE(_context.logger, "Rebuilding swapchain...");

        if (_frameCount != 0){
            if (vkWaitForFences(
                device,
                1,
                &_frames[_currentFrame].inFlightFence,
                VK_TRUE,
                UINT64_MAX
            ) != VK_SUCCESS){
                SPDLOG_LOGGER_ERROR(_context.logger, "Failed to wait for fence");
                throw std::runtime_error("Failed to wait for fence");
            }
        }

        querySupport();

        _surfaceFormat = findSurfaceFormat();
        _extent = findExtent();
        _presentMode = findPresentMode();
        _frameCount = findFrameCount();
        
        const bool formatChanged = _surfaceFormat.format != _buildInfo.surfaceFormat.format;
        const bool colorSpaceChanged = _surfaceFormat.colorSpace != _buildInfo.surfaceFormat.colorSpace;
        const bool frameCountChanged = _frameCount != _buildInfo.frameCount;
        const bool presentModeChanged = _presentMode != _buildInfo.presentMode;
        const bool sizeChanged = (_extent.width != _buildInfo.extent.width) || (_extent.height != _buildInfo.extent.height);

        if (_swapchain){
            if (formatChanged || colorSpaceChanged){
                SPDLOG_LOGGER_ERROR(_context.logger, "The swapchain format and or color space changed !");
                throw std::runtime_error("Failed to rebuild swapchain");
            }
        } else {
            createCommandPool();
            createRenderPass();
        }

        VkSwapchainKHR oldSwapchain = _swapchain;

        VkSwapchainCreateInfoKHR info{};
        info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

		info.surface = _surface;
		info.oldSwapchain = oldSwapchain;
		info.preTransform = _support.capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = _presentMode;
		info.clipped = VK_TRUE;

		// uint32_t queueFamilyIndices[] = {
		// 	_context.graphics.family,
		// 	_context.present.family,
		// };

		// if (queueFamilyIndices[0] != queueFamilyIndices[1]){
		// 	info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		// 	info.queueFamilyIndexCount = 2;
		// 	info.pQueueFamilyIndices = queueFamilyIndices;
		// } else {
		// 	info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		// 	info.queueFamilyIndexCount = 0;
		// 	info.pQueueFamilyIndices = nullptr;
		// }

		info.minImageCount = _frameCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (vkCreateSwapchainKHR(device, &info, nullptr, &_swapchain) != VK_SUCCESS){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create swapchain");
			throw std::runtime_error("Failed to create swapchain");
		}

        _currentFrame = 0;

        vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
        getSwapchainFrames();
    }

    void WindowContext::resetSwapchainFrames(){
        auto& device = _context.device;

        for (auto& frame : _frames){
            if (frame.framebuffer != VK_NULL_HANDLE){
				vkDestroyFramebuffer(device, frame.framebuffer, nullptr);
				frame.framebuffer = VK_NULL_HANDLE;
			}

			if (frame.imageView != VK_NULL_HANDLE){
				vkDestroyImageView(device, frame.imageView, nullptr);
				frame.imageView = VK_NULL_HANDLE;
			}

			if (frame.imageAvailable != VK_NULL_HANDLE){
				vkDestroySemaphore(device, frame.imageAvailable, nullptr);
				frame.imageAvailable = VK_NULL_HANDLE;
			}

			if (frame.imageFinished != VK_NULL_HANDLE){
				vkDestroySemaphore(device, frame.imageFinished, nullptr);
				frame.imageFinished = VK_NULL_HANDLE;
			}

			if (frame.inFlightFence != VK_NULL_HANDLE){
				vkDestroyFence(device, frame.inFlightFence, nullptr);
				frame.inFlightFence = VK_NULL_HANDLE;
			}
        }

        if (!_commandBuffers.empty()){
            vkFreeCommandBuffers(
                device,
                _commandPool,
                static_cast<uint32_t>(_commandBuffers.size()),
                _commandBuffers.data()
            );
        

            _commandBuffers.clear();
        }
    }
    
    void WindowContext::getSwapchainFrames(){
        resetSwapchainFrames();

        _frames.resize(_frameCount);

        getSwapchainImages();
        createImageViews();
        createFramebuffers();
        createSyncObjects();
        allocateCommandBuffers();
    }

    void WindowContext::getSwapchainImages(){
        auto& device = _context.device;

		uint32_t imageCount = 0;
		if (vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, nullptr) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}

		if (imageCount != _frameCount){
            SPDLOG_LOGGER_INFO(_context.logger, "The number of swapchain frame changed from {} to {}", _frameCount, imageCount);
		}

		_frameCount = imageCount;

		std::vector<VkImage> images(_frameCount);
		if (vkGetSwapchainImagesKHR(device, _swapchain, &_frameCount, images.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to get swapchain images");
		}

		for (uint32_t i=0; i<_frameCount; i++){
			_frames[i].image = images[i];
		}
    }

    void WindowContext::createImageViews(){
        auto& device = _context.device;

		for (auto &frame : _frames){
			VkImageViewCreateInfo viewInfo{};

			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = frame.image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = _surfaceFormat.format;

			viewInfo.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			if (vkCreateImageView(device, &viewInfo, nullptr, &frame.imageView) != VK_SUCCESS){
                SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create swapchain attachment image view");
				throw std::runtime_error("Failed to create swapchain attachment image view");
			}
		}
    }

    void WindowContext::createFramebuffers(){
        auto& device = _context.device;

		for (auto &frame : _frames){
			VkFramebufferCreateInfo framebufferInfo = {};

			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &frame.imageView;
			framebufferInfo.width = _extent.width;
			framebufferInfo.height = _extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &frame.framebuffer) != VK_SUCCESS){
				throw std::runtime_error("Failed to create swapchain framebuffer");
			}
		}
    }

    void WindowContext::createSyncObjects(){
        auto& device = _context.device;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (auto &frame : _frames) {
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &frame.imageAvailable) != VK_SUCCESS){
				throw std::runtime_error("Failed to create swapchain attachment image available semahore");
			}

			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &frame.imageFinished) != VK_SUCCESS){
				throw std::runtime_error("Failed to create swapchain attachment image available semahore");
			}
			
			if (vkCreateFence(device, &fenceInfo, nullptr, &frame.inFlightFence) != VK_SUCCESS){
				throw std::runtime_error("Failed to create swapchain attachment in flight fence");
			}
		}
    }

    void WindowContext::allocateCommandBuffers(){
        auto& device = _context.device;

        VkCommandBufferAllocateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = _commandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = _frameCount;

        _commandBuffers.resize(_frameCount);

        if (vkAllocateCommandBuffers(device, &info, _commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate command buffers");
        }
    }
    
	VkSurfaceFormatKHR WindowContext::findSurfaceFormat(){
		const auto& wanted = _buildInfo.surfaceFormat;

        // Find the best format, at least the one that corresponds the most the wanted one

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

	VkPresentModeKHR WindowContext::findPresentMode(){
		const auto& wanted = _buildInfo.presentMode;

		for (const auto& mode : _support.presentModes){
			if (mode == wanted){
				return mode;
			}
		}

		return _support.presentModes[0];
	}

	VkExtent2D WindowContext::findExtent(){
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

	uint32_t WindowContext::findFrameCount(){
		const auto& capabilities = _support.capabilities;
		return std::clamp(_buildInfo.frameCount, capabilities.minImageCount, capabilities.maxImageCount == 0 ? UINT32_MAX : capabilities.maxImageCount);
	}

    void WindowContext::querySupport(){
		querySupportCapabilities();
		querySupportFormats();
		querySupportPresentModes();
	}
    
	void WindowContext::querySupportCapabilities(){
		const auto& physicalDevice = _context.physicalDevice;

		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &_support.capabilities) != VK_SUCCESS){
			throw std::runtime_error("Failed to query surface swapchain capabilities");
		}
	}

	void WindowContext::querySupportFormats(){
		const auto& physicalDevice = _context.physicalDevice;

		uint32_t count = 0;

		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &count, nullptr) != VK_SUCCESS){
			throw std::runtime_error("Failed to query surface swapchain formats");
		}

		_support.formats.resize(static_cast<std::size_t>(count));

		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &count, _support.formats.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to query surface swapchain formats");
		}
	}

	void WindowContext::querySupportPresentModes(){
		const auto& physicalDevice = _context.physicalDevice;

		uint32_t count = 0;

		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &count, nullptr) != VK_SUCCESS){
			throw std::runtime_error("Failed to query surface swapchain present modes");
		}

		_support.presentModes.resize(static_cast<std::size_t>(count));

		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &count, _support.presentModes.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to query surface swapchain present modes");
		}
	}

    WindowContext& WindowContext::wantExtent(VkExtent2D extent){
        _buildInfo.extent = extent;
        return *this;
    }

    WindowContext& WindowContext::wantFrameCount(uint32_t count){
        _buildInfo.frameCount = count;
        return *this;
    }

    WindowContext& WindowContext::wantPresentMode(VkPresentModeKHR presentMode){
        _buildInfo.presentMode = presentMode;
        return *this;
    }

    WindowContext& WindowContext::wantSurfaceFormat(VkSurfaceFormatKHR surfaceFormat){
        _buildInfo.surfaceFormat = surfaceFormat;
        return *this;
    }
    
    VkCommandBuffer WindowContext::beginCommandBuffer(){
        auto window = _window.lock();

        if (!window){
            throw std::runtime_error("Window is invalid");
        }

        VkResult result = aquireNextImage();

        bool windowResized = false;
        if (windowResized || result == VK_ERROR_OUT_OF_DATE_KHR){
            rebuildSwapchain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to acquire next image");
            throw std::runtime_error("Failed to acquire next image");
        }

        VkCommandBuffer commandBuffer = _commandBuffers[_currentFrame];

        VkCommandBufferBeginInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(commandBuffer, &info) != VK_SUCCESS){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to begin command buffer");
            throw std::runtime_error("Failed to begin command buffer");
        }
		
        return commandBuffer;
    }

    void WindowContext::submitCommandBuffer(){
        auto window = _window.lock();
		auto& device = _context.device;

        // Ensure the window is valid
        if (!window){
            throw std::runtime_error("Window is invalid");
        }

        // End the command buffer
        VkCommandBuffer commandBuffer = _commandBuffers[_currentFrame];
        VkResult result = vkEndCommandBuffer(commandBuffer);
        
        // Check recording
        if (result != VK_SUCCESS){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to end command buffer");
            throw std::runtime_error("Failed to end command buffer");
        }

        // Wait for the next frame to be available
        if (_frames[_nextFrame].imageInFlight != VK_NULL_HANDLE){
			if (vkWaitForFences(
                device,
                1,
                &_frames[_nextFrame].imageInFlight,
                VK_TRUE,
                UINT64_MAX
            ) != VK_SUCCESS){
                SPDLOG_LOGGER_ERROR(_context.logger, "Failed to wait for swapchain attachment in flight fence");
				throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
			}
		}
        
        
        // Set next frame's fence
        _frames[_nextFrame].imageInFlight = _frames[_currentFrame].inFlightFence;

        VkSemaphore waitSemaphores[] = {_frames[_currentFrame].imageAvailable};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores[] = {_frames[_currentFrame].imageFinished};

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

        // Reset the fence
		if (vkResetFences(
            device,
            1,
            &_frames[_currentFrame].inFlightFence
        ) != VK_SUCCESS){
			throw std::runtime_error("Failed to reset swapchain attachment in flight fence");
		}

        // Submit the command buffer
        // if (vkQueueSubmit(
        //     _context.graphics.queue,
        //     1,
        //     &submitInfo,
        //     _frames[_currentFrame].inFlightFence
        // ) != VK_SUCCESS){
		// 	throw std::runtime_error("Faile to submit graphics command buffer");
		// }

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &_swapchain;

		presentInfo.pImageIndices = &_nextFrame;

        // present the frame when the render is ready
		// result = vkQueuePresentKHR(_context.present.queue, &presentInfo);
		_currentFrame = (_currentFrame + 1) % _frameCount;
        
        bool windowResized = false;
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowResized){
            rebuildSwapchain();
        } else if (result != VK_SUCCESS) {
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to present swapchain image");
            throw std::runtime_error("Failed to present swapchain image");
        }
    }

    VkResult WindowContext::aquireNextImage(){
        auto& device = _context.device;

		if (vkWaitForFences(
                device,
                1,
                &_frames[_currentFrame].inFlightFence,
                VK_TRUE,
                UINT64_MAX
            ) != VK_SUCCESS){
			throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
		}

		VkResult result = vkAcquireNextImageKHR(
            device,
            _swapchain,
            UINT64_MAX,
            _frames[_currentFrame].imageAvailable,
            VK_NULL_HANDLE,
            &_nextFrame
        );

		return result;
    }

    void WindowContext::beginRenderPass(VkCommandBuffer commandBuffer){
        VkRenderPassBeginInfo renderPassBeginInfo{};

		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = _renderPass;
		renderPassBeginInfo.framebuffer = _frames[_currentFrame].framebuffer;

		renderPassBeginInfo.renderArea.offset = {0, 0};
		renderPassBeginInfo.renderArea.extent = _extent;

		VkClearValue clearValue = VkClearValue{_clearColor};

		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width =  static_cast<float>(_extent.width);
		viewport.height = static_cast<float>(_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0, 0}, _extent};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void WindowContext::endRenderPass(VkCommandBuffer commandBuffer){
        vkCmdEndRenderPass(commandBuffer);
    }

    uint32_t WindowContext::getFrameCount() const{
        return _frameCount;
    }

    uint32_t WindowContext::getCurrentFrame() const{
        return _currentFrame;
    }

    uint32_t WindowContext::getNextFrame() const{
        return _nextFrame;
    }

    const WindowContext::FrameData& WindowContext::getFrameData(uint32_t frame) const{
        return _frames[frame];
    }
}