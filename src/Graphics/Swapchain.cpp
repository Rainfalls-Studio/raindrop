#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	Swapchain::Swapchain(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Swapchain");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.Swapchain") << "Creating swapchain...";

		_swapchainSupport = _context.device.getSwapchainSupport(_context.window.surface());

		findSurfaceFormat();
		createRenderPass();	
		rebuildSwapchain();

		CLOG(INFO, "Engine.Graphics.Swapchain") << "Created swapchain with success !";
	}

	Swapchain::~Swapchain(){
		CLOG(INFO, "Engine.Graphics.Swapchain") << "Destroying swapchain...";

		_oldSwapchain.reset();
		_swapchain.reset();
		if (_renderPass) vkDestroyRenderPass(_context.device.get(), _renderPass, _context.allocationCallbacks);

		CLOG(INFO, "Engine.Graphics.Swapchain") << "Swapchain destroyed with success !";
	}
	
	void Swapchain::setExtent(VkExtent2D extent){
		_wantedExtent = extent;
	}

	void Swapchain::setFrameCount(uint32_t count){
		_wantedFrameCount = count;
	}

	void Swapchain::setPresentMode(VkPresentModeKHR presentMode){
		_wantedPresentMode = presentMode;
	}

	void Swapchain::setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat){
		_wantedSurfaceFormat = surfaceFormat;
	}

	void Swapchain::rebuildSwapchain(){
		CLOG(INFO, "Engine.Graphics.Swapchain") << "Building/Rebuilding swapchain...";

		_context.device.waitIdle();
		
		std::swap(_swapchain, _oldSwapchain);
		_swapchain = std::make_unique<SwapchainData>(_context);

		_swapchainSupport = _context.device.getSwapchainSupport(_context.window.surface());
		auto surfaceFormat = _surfaceFormat;

		findSurfaceFormat();
		bool hasColorSpaceChanged = surfaceFormat.colorSpace != _surfaceFormat.colorSpace;
		bool hasFormatChanged = surfaceFormat.format != _surfaceFormat.format;

		if (hasColorSpaceChanged || hasFormatChanged){
			CLOG(ERROR, "Engine.Graphics.Swapchain") << "The swapchain surface format and/or color space has changed";
			throw std::runtime_error("The surface format and/or color space has changed");
		}

		findPresentMode();
		findFrameCount();
		findExtent();
		
		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.surface = _context.window.surface();
		info.oldSwapchain = _oldSwapchain != nullptr ? _oldSwapchain->_swapchain : VK_NULL_HANDLE;
		info.preTransform = _swapchainSupport.capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = _presentMode;
		info.clipped = VK_TRUE;
		
		info.minImageCount = _frameCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (vkCreateSwapchainKHR(_context.device.get(), &info, _context.allocationCallbacks, &_swapchain->_swapchain) != VK_SUCCESS){
			throw std::runtime_error("Failed to create window vulkan swapchain !");
		}

		getSwapchainImages();
		createImageViews();
		createFramebuffers();
		createSyncObjects();
		_currentFrame = 0;

		CLOG(INFO, "Engine.Graphics.Swapchain") << "Swapchain Built/Rebuilt with success !";
	}

	void Swapchain::findSurfaceFormat(){
		for (auto& f : _swapchainSupport.formats){
			if (f.colorSpace == _wantedSurfaceFormat.colorSpace && f.format == _wantedSurfaceFormat.format){
				_surfaceFormat = _wantedSurfaceFormat;
				_imageFormat = _surfaceFormat.format;
				return;
			}
		}
		_surfaceFormat = _swapchainSupport.formats[0];
		_imageFormat = _surfaceFormat.format;
	}

	void Swapchain::findPresentMode(){
		for (auto& p : _swapchainSupport.presentModes){
			if (p == _wantedPresentMode){
				_presentMode = _wantedPresentMode;
			}
		}
		_presentMode = _swapchainSupport.presentModes[0];
	}

	void Swapchain::findExtent(){
		auto& capabilities = _swapchainSupport.capabilities;

		// if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
		// 	printf("aa\n");
		// 	_extent = capabilities.currentExtent;
		// 	return;
		// }

		_extent.width = std::clamp(_wantedExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		_extent.height = std::clamp(_wantedExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return;
	}

	void Swapchain::findFrameCount(){
		auto& capabilities = _swapchainSupport.capabilities;

		uint32_t oldFrameCount = _frameCount;
		_frameCount = std::clamp(_wantedFrameCount, capabilities.minImageCount+1, capabilities.maxImageCount);

		if (oldFrameCount != _frameCount){
			_swapchain->_frames.resize(_frameCount);
		}
	}

	void Swapchain::createRenderPass(){
		auto& registry = _context->registry;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = _imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_context.device.get(), &renderPassInfo, _context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
			throw std::runtime_error("Failed to create swapchain's render pass");
		}
		
		registry["Engine.Graphics.Swapchain.RenderPass"] = _renderPass;
	}

	void Swapchain::createImageViews(){
		for (auto &f : _swapchain->_frames){
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = f.image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = _imageFormat;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(_context.device.get(), &viewInfo, _context.allocationCallbacks, &f.imageView) !=
				VK_SUCCESS) {
				throw std::runtime_error("failed to create texture image view!");
			}
		}
	}

	void Swapchain::getSwapchainImages(){
		vkGetSwapchainImagesKHR(_context.device.get(), _swapchain->_swapchain, &_frameCount, nullptr);
		std::vector<VkImage> images(_frameCount);
		vkGetSwapchainImagesKHR(_context.device.get(), _swapchain->_swapchain, &_frameCount, images.data());

		_swapchain->_frames.resize(_frameCount);

		for (uint32_t i=0; i<_frameCount; i++){
			_swapchain->_frames[i].image = images[i];
		}
	}

	void Swapchain::createFramebuffers(){
		for (auto &f : _swapchain->_frames){
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &f.imageView;
			framebufferInfo.width = _extent.width;
			framebufferInfo.height = _extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(_context.device.get(), &framebufferInfo, _context.allocationCallbacks, &f.framebuffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to create vulkan swapchain framebuffer");
			}
		}
	}

	void Swapchain::createSyncObjects(){
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (auto &f : _swapchain->_frames) {
			if (vkCreateSemaphore(_context.device.get(), &semaphoreInfo, _context.allocationCallbacks, &f.imageAvailable) != VK_SUCCESS)
				throw std::runtime_error("failed to create vulkan swapchain semaphore");
			
			if (vkCreateSemaphore(_context.device.get(), &semaphoreInfo, _context.allocationCallbacks, &f.imageFinished) != VK_SUCCESS)
				throw std::runtime_error("failed to create vulkan swapchain semaphore");
			
			if (vkCreateFence(_context.device.get(), &fenceInfo, _context.allocationCallbacks, &f.inFlightFence) != VK_SUCCESS)
				throw std::runtime_error("failed to create vulkan swapchain fence");
		}
	}
	
	VkResult Swapchain::acquireNextImage(){
		uint32_t index;
		vkWaitForFences(_context.device.get(), 1, &_swapchain->_frames[_currentFrame].inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		VkResult result = vkAcquireNextImageKHR(_context.device.get(), _swapchain->_swapchain, std::numeric_limits<uint64_t>::max(), _swapchain->_frames[_currentFrame].imageAvailable, VK_NULL_HANDLE, &index);
		return result;
	}

	VkResult Swapchain::submitCommandBuffer(VkCommandBuffer* buffers){
		if (_swapchain->_frames[_currentFrame].imageInFlight != VK_NULL_HANDLE)
			vkWaitForFences(_context.device.get(), 1, &_swapchain->_frames[_currentFrame].imageInFlight, VK_TRUE, UINT64_MAX);
		
		_swapchain->_frames[_currentFrame].imageInFlight = _swapchain->_frames[_currentFrame].inFlightFence;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {_swapchain->_frames[_currentFrame].imageAvailable};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffers;

		VkSemaphore signalSemaphores[] = {_swapchain->_frames[_currentFrame].imageFinished};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(_context.device.get(), 1, &_swapchain->_frames[_currentFrame].inFlightFence);
		if (vkQueueSubmit(_context.graphicsQueue, 1, &submitInfo, _swapchain->_frames[_currentFrame].inFlightFence) != VK_SUCCESS)
			throw "failed to submit draw command buffer";

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {_swapchain->_swapchain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &_currentFrame;

		auto result = vkQueuePresentKHR(_context.presentQueue, &presentInfo);

		_currentFrame = (_currentFrame + 1) % _frameCount;

		return result;
	}

	uint32_t Swapchain::frameCount() const{
		return _frameCount;
	}

	VkRenderPass Swapchain::renderPass() const{
		return _renderPass;
	}

	uint32_t Swapchain::currentFrame() const{
		return _currentFrame;
	}

	void Swapchain::beginRenderPass(VkCommandBuffer commandBuffer){
		VkRenderPassBeginInfo renderPassBeginInfo{};

		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = _renderPass;
		renderPassBeginInfo.framebuffer = _swapchain->_frames[_currentFrame].framebuffer;

		renderPassBeginInfo.renderArea.offset = {0, 0};
		renderPassBeginInfo.renderArea.extent = _extent;

		VkClearValue clearValue = VkClearValue{_clearColor};

		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width =  _extent.width;
		viewport.height = _extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0, 0}, _extent};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Swapchain::endRenderPass(VkCommandBuffer commandBuffer){
    	vkCmdEndRenderPass(commandBuffer);
	}

	void Swapchain::setClearColor(VkClearColorValue color){
		_clearColor = color;
	}
	
	Swapchain::Frame& Swapchain::getFrameData(uint32_t id){
		return _swapchain->_frames[id];
	}

	Swapchain::Frame& Swapchain::getCurrentFrameData(){
		return _swapchain->_frames[currentFrame()];
	}

	std::vector<Swapchain::Frame>& Swapchain::getFramesData(){
		return _swapchain->_frames;
	}
	
	Swapchain::SwapchainData::~SwapchainData(){
		_context.device.waitIdle();
		for (auto &frame : _frames){
			if (frame.framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(_context.device.get(), frame.framebuffer, _context.allocationCallbacks);
			if (frame.imageView != VK_NULL_HANDLE) vkDestroyImageView(_context.device.get(), frame.imageView, _context.allocationCallbacks);
			if (frame.imageAvailable != VK_NULL_HANDLE) vkDestroySemaphore(_context.device.get(), frame.imageAvailable, _context.allocationCallbacks);
			if (frame.imageFinished != VK_NULL_HANDLE) vkDestroySemaphore(_context.device.get(), frame.imageFinished, _context.allocationCallbacks);
			if (frame.inFlightFence != VK_NULL_HANDLE) vkDestroyFence(_context.device.get(), frame.inFlightFence, _context.allocationCallbacks);
		}
		if (_swapchain) vkDestroySwapchainKHR(_context.device.get(), _swapchain, _context.allocationCallbacks);
	}

	Swapchain::SwapchainData::SwapchainData(GraphicsContext& context) : _context{context}{}
}