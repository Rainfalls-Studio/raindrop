#include "Swapchain.hpp"

Swapchain::Swapchain(GfxAllocator& allocator, VkSurfaceKHR surface, Device &device, uint32 familyIndex) : _allocator{allocator}, _device{device}, _frames(allocator.get()), _surface{surface}{
	RAINDROP_profile_function();
	_previous = nullptr;
	defaultSettings();
}

Swapchain::Swapchain(GfxAllocator& allocator, VkSurfaceKHR surface, Device &device, Swapchain* previous) : _allocator{allocator}, _device{device}, _frames(allocator.get()), _previous{previous}, _surface{surface}{
	RAINDROP_profile_function();
	_bufferCount = previous->_bufferCount;
	_depthBufferRequired = previous->_depthBufferRequired;
	_vsync = _previous->_vsync;
	_renderQuality = _previous->_renderQuality;
	_colorFormat = _previous->_colorFormat;
	_depthFormat = _previous->_depthFormat;
	_colorSpace = _previous->_colorSpace;
}

Swapchain::~Swapchain(){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_bufferCount; i++){
		auto& frame = _frames[i];

		if (frame._framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(_device.get(), frame._framebuffer, _allocator.callback());
		if (frame._depth != VK_NULL_HANDLE) vkDestroyImage(_device.get(), frame._depth, _allocator.callback());
		if (frame._imageView != VK_NULL_HANDLE) vkDestroyImageView(_device.get(), frame._imageView, _allocator.callback());
		if (frame._depthView != VK_NULL_HANDLE) vkDestroyImageView(_device.get(), frame._depthView, _allocator.callback());
		if (frame._imageMemory != VK_NULL_HANDLE) vkFreeMemory(_device.get(), frame._imageMemory, _allocator.callback());
		if (frame._depthMemory != VK_NULL_HANDLE) vkFreeMemory(_device.get(), frame._depthMemory, _allocator.callback());
		if (frame._imageAvailable != VK_NULL_HANDLE) vkDestroySemaphore(_device.get(), frame._imageAvailable, _allocator.callback());
		if (frame._imageFinished != VK_NULL_HANDLE) vkDestroySemaphore(_device.get(), frame._imageFinished, _allocator.callback());
		if (frame._inFlightFence != VK_NULL_HANDLE) vkDestroyFence(_device.get(), frame._inFlightFence, _allocator.callback());

		frame._framebuffer = VK_NULL_HANDLE;
		frame._depth = VK_NULL_HANDLE;
		frame._imageView = VK_NULL_HANDLE;
		frame._depthView = VK_NULL_HANDLE;
		frame._imageMemory = VK_NULL_HANDLE;
		frame._depthMemory = VK_NULL_HANDLE;
		frame._imageInFlight = VK_NULL_HANDLE;
		frame._imageAvailable = VK_NULL_HANDLE;
		frame._imageFinished = VK_NULL_HANDLE;
		frame._inFlightFence = VK_NULL_HANDLE;
	}
	
	if (_swapchain != VK_NULL_HANDLE) vkDestroySwapchainKHR(_device.get(), _swapchain, _allocator.callback());
	_swapchain = VK_NULL_HANDLE;
}

void Swapchain::defaultSettings(){
	RAINDROP_profile_function();
	_bufferCount = 2;
	_depthBufferRequired = true;
	_vsync = true;
	_renderQuality = {1.f, 1.f};
	_colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
	_depthFormat = findSupportedDepthFormat();
	_colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}

void Swapchain::setBufferCount(uint32 bufferCount){
	RAINDROP_profile_function();
	_bufferCount = bufferCount;
}

void Swapchain::enableVSync(bool enable){
	RAINDROP_profile_function();
	_vsync = enable;
}

void Swapchain::requireDepthBuffer(bool require){
	RAINDROP_profile_function();
	_depthBufferRequired = require;
}

void Swapchain::setRenderQuality(maths::vec2<float32> quality){
	RAINDROP_profile_function();
	_renderQuality = quality;
}

void Swapchain::setRequiredColorFormat(VkFormat colorFormat){
	RAINDROP_profile_function();
	_colorFormat = colorFormat;
}

void Swapchain::setRequiredDepthFormat(VkFormat depthFormat){
	RAINDROP_profile_function();
	_depthFormat = depthFormat;
}

void Swapchain::setRequiredColorSpace(VkColorSpaceKHR colorSpace){
	RAINDROP_profile_function();
	_colorSpace = colorSpace;
}

VkFormat Swapchain::findSupportedDepthFormat(){
	RAINDROP_profile_function();
	Array<VkFormat> formats(_allocator.stack(), 3);
	formats[0] = VK_FORMAT_D32_SFLOAT;
	formats[1] = VK_FORMAT_D32_SFLOAT_S8_UINT;
	formats[2] = VK_FORMAT_D24_UNORM_S8_UINT;
	return _device.physicalDevice().getFirstSupportedFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void Swapchain::setExtent(VkExtent2D extent){
	RAINDROP_profile_function();
	_extent = extent;
}

void Swapchain::updatePresentMode(Array<VkPresentModeKHR>& presentModes){
	RAINDROP_profile_function();
	if (!_vsync){
		for (uint32 i=0; i<presentModes.size(); i++){
			if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR){
				_presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				return;
			}
		}
		RAINDROP_log(WARNING, GRAPHICS, "failed to create a vulkan pipeline with imedtate present mode");
	}
	_presentMode = VK_PRESENT_MODE_FIFO_KHR;
}

uint32 Swapchain::getGraphicsFamilyIndex(){
	RAINDROP_profile_function();
	const auto& families = _device.physicalDevice().getFamiliesProperties();

	for (uint32 i=0; i<families.size(); i++){
		const auto& family = families[i];
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) return i;
	}
	RAINDROP_log(ERROR, GRAPHICS, "failed to find a graphics vulkan family for the swapchain");
	return -1;
}

uint32 Swapchain::getPresentFamilyIndex(){
	RAINDROP_profile_function();
	const auto& families = _device.physicalDevice().getFamiliesProperties();

	for (uint32 i=0; i<families.size(); i++){
		if (_device.physicalDevice().isPresentSupported(i, _surface)) return i;
	}
	RAINDROP_log(ERROR, GRAPHICS, "failed to find a present vulkan family for the swapchain");
	return -1;
}

void Swapchain::init(){
	RAINDROP_profile_function();

	_loaded = false;
	if (!createSwapchain()) return;
	if (!createImageViews()) return;
	if (!createDepthResources()) return;
	if (!_previous){
		if (!createRenderPass()) return;
	}
	if (!createFramebuffers()) return;
	if (!createSyncObjects()) return;
	_loaded = true;

	_previous = nullptr;
	RAINDROP_log(TRACE, GRAPHICS, "vulkan swapchain created with success");
}

bool Swapchain::createSwapchain(){
	RAINDROP_profile_function();

	const auto& physicalDevice = _device.physicalDevice();
	VkSurfaceCapabilitiesKHR capabilities = physicalDevice.getSurfaceCapabilities(_surface);
	Array<VkPresentModeKHR> presentModes = physicalDevice.getSurfacePresentModes(_allocator.stack(), _surface);

	updatePresentMode(presentModes);
	
	_bufferCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && _bufferCount > capabilities.maxImageCount){
		_bufferCount = capabilities.maxImageCount;
	}

	_frames.resize(_bufferCount);

	VkSwapchainCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = _surface;

	info.minImageCount = _bufferCount;
	info.imageFormat = _colorFormat;
	info.imageColorSpace = _colorSpace;
	info.imageExtent = _extent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	info.preTransform = capabilities.currentTransform;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.presentMode = _presentMode;
	info.clipped = VK_TRUE;
	info.oldSwapchain = _previous == nullptr ? VK_NULL_HANDLE : _previous->_swapchain;

	uint32 familyIndices[] = {getGraphicsFamilyIndex(), getPresentFamilyIndex()};
	if (familyIndices[0] != familyIndices[1]){
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = familyIndices;
	} else {
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
	}

	VkResult result = vkCreateSwapchainKHR(_device.get(), &info, _allocator.callback(), &_swapchain);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan swapchain : %s", string_VkResult(result));
		return false;
	}

	vkGetSwapchainImagesKHR(_device.get(), _swapchain, &_bufferCount, nullptr);
	Array<VkImage> images(_allocator.stack(), _bufferCount);
	vkGetSwapchainImagesKHR(_device.get(), _swapchain, &_bufferCount, images.data());

	for (uint32 i=0; i<_bufferCount; i++){
		_frames[i]._image = images[i];
	}
	return true;
}

bool Swapchain::createImageViews(){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_bufferCount; i++){
		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _frames[i]._image;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = _colorFormat;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(_device.get(), &info, _allocator.callback(), &_frames[i]._imageView);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create %d vulkan swapchain image view : %s", i, string_VkResult(result));
			return false;
		}
	}
	return true;
}

bool Swapchain::createRenderPass(){
	RAINDROP_profile_function();
	VkAttachmentDescription depthAttachmentDescription = getDepthAttachment();
	VkAttachmentDescription colorAttachmentDescription = getColorAttachment();
	
	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 0;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 1;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstSubpass = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription descriptions[] = {depthAttachmentDescription, colorAttachmentDescription};
	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = sizeof(descriptions) / sizeof(VkAttachmentDescription);
	info.pAttachments = descriptions;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;

	VkResult result = vkCreateRenderPass(_device.get(), &info, _allocator.callback(), &_renderPass);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan swapchain render pass : %s", string_VkResult(result));
		return false;
	}
	return true;
}

bool Swapchain::createDepthResources(){
	RAINDROP_profile_function();
	if (!createDepthImages()) return false;
	if (!createDepthImageView()) return false;
	return true;
}

bool Swapchain::createDepthImages(){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_bufferCount; i++){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.extent.width = _extent.width;
		info.extent.height = _extent.height;
		info.extent.depth = 1;
		info.mipLevels = 1;
		info.arrayLayers = 1;
		info.format = _depthFormat;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.flags = 0;

		if (!_device.createImageWithInfo(info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _frames[i]._depth, _frames[i]._depthMemory)){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create %d vulkan swapchain depth image", i);
			return false;
		}
	}
	return true;
}

bool Swapchain::createDepthImageView(){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_bufferCount; i++){
		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _frames[i]._depth;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = _depthFormat;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(_device.get(), &info, _allocator.callback(), &_frames[i]._depthView);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create %d vulkan swapchain image view", i);
			return false;
		}
	}
	return true;
}

bool Swapchain::createFramebuffers(){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_bufferCount; i++){
		VkImageView attachments[] = {_frames[i]._imageView, _frames[i]._depthView};

		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO;
		info.renderPass = _renderPass;
		info.attachmentCount = 2;
		info.pAttachments = attachments;
		info.width = _extent.width;
		info.height = _extent.height;
		info.layers = 1;

		VkResult result = vkCreateFramebuffer(_device.get(), &info, _allocator.callback(), &_frames[i]._framebuffer);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create %d vulkan swapchain frame buffer", i);
			return false;
		}
	}
	return true;
}

bool Swapchain::createSyncObjects(){
	RAINDROP_profile_function();

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32 i=0; i<_bufferCount; i++){
		auto& frame = _frames[i];

		VkResult result = vkCreateFence(_device.get(), &fenceInfo, _allocator.callback(), &frame._inFlightFence);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create swapchain fence : %s", string_VkResult(result));
			return false;
		}

		result = vkCreateSemaphore(_device.get(), &semaphoreInfo, _allocator.callback(), &frame._imageAvailable);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create swapchain semaphore : %s", string_VkResult(result));
			return false;
		}

		result = vkCreateSemaphore(_device.get(), &semaphoreInfo, _allocator.callback(), &frame._imageFinished);
		if (result != VK_SUCCESS){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create swapchain semaphore : %s", string_VkResult(result));
			return false;
		}
	}
	return true;
}

VkAttachmentDescription Swapchain::getDepthAttachment(){
	RAINDROP_profile_function();
	VkAttachmentDescription attachment{};
	attachment.format = _depthFormat;
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return attachment;
}

VkAttachmentDescription Swapchain::getColorAttachment(){
	RAINDROP_profile_function();
	VkAttachmentDescription attachment{};
	attachment.format = _depthFormat;
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	return attachment;
}

VkRenderPass Swapchain::renderPass() const{
	RAINDROP_profile_function();
	return _renderPass;
}

bool Swapchain::vsync() const{
	RAINDROP_profile_function();
	return _vsync;
}

uint32 Swapchain::bufferCount() const{
	RAINDROP_profile_function();
	return _bufferCount;
}

VkFormat Swapchain::depthFormat() const{
	RAINDROP_profile_function();
	return _depthFormat;
}

VkFormat Swapchain::colorFormat() const{
	RAINDROP_profile_function();
	return _colorFormat;
}

uint32 Swapchain::currentBuffer() const{
	RAINDROP_profile_function();
	return _currentFrame;
}

maths::vec2<float32> Swapchain::renderQuality() const{
	RAINDROP_profile_function();
	return _renderQuality;
}

bool Swapchain::loaded() const{
	RAINDROP_profile_function();
	return _loaded;
}

void Swapchain::beginRenderPass(VkCommandBuffer commandBuffer){
	RAINDROP_profile_function();
	const auto& frame = _frames[_currentFrame];

	VkRenderPassBeginInfo info{};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = _renderPass;
	info.framebuffer = frame._framebuffer;
	info.renderArea.offset = {0, 0};
	info.renderArea.extent = _extent;

	VkClearValue clearValues[2];
	clearValues[0].depthStencil = {1.f, 0};
	clearValues[1].color.float32[0] = _clearColor.x;
	clearValues[1].color.float32[1] = _clearColor.y;
	clearValues[1].color.float32[2] = _clearColor.z;
	clearValues[1].color.float32[3] = _clearColor.w;

	info.clearValueCount = 2;
	info.pClearValues = clearValues;

	vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = _extent.width;
	viewport.height = _extent.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = _extent;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Swapchain::endRenderPass(VkCommandBuffer commandBuffer){
	RAINDROP_profile_function();
	vkCmdEndRenderPass(commandBuffer);
}

VkResult Swapchain::submitCommandBuffer(VkCommandBuffer* buffers){
	RAINDROP_profile_function();
	auto& frame = _frames[_currentFrame];

	if (frame._imageInFlight != VK_NULL_HANDLE)
		vkWaitForFences(_device.get(), 1, &frame._imageInFlight, VK_TRUE, UINT64_MAX);
	

	frame._imageInFlight = frame._inFlightFence;

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {frame._imageAvailable};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = buffers;

	VkSemaphore signalSemaphores[] = {frame._imageFinished};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(_device.get(), 1, &frame._inFlightFence);

	uint32 graphicsFamily = getGraphicsFamilyIndex();
	if (graphicsFamily == INVALID_FAMILY){
		RAINDROP_log(ERROR, GRAPHICS, "failed to found a family that support graphics queue");
		return VK_ERROR_UNKNOWN;
	}

	VkQueue queue = _device.getQueue(graphicsFamily);
	if (queue == VK_NULL_HANDLE){
		RAINDROP_log(ERROR, GRAPHICS, "failed to get a graphics queue");
		return VK_ERROR_UNKNOWN;
	}

	if (vkQueueSubmit(queue, 1, &submitInfo, frame._inFlightFence) != VK_SUCCESS)
		throw "failed to submit draw command buffer";

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {_swapchain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &_currentFrame;

	uint32 presentFamily = getPresentFamilyIndex();
	if (presentFamily == INVALID_FAMILY){
		RAINDROP_log(ERROR, GRAPHICS, "failed to found a family that support present");
		return VK_ERROR_UNKNOWN;
	}

	VkQueue presentQueue = _device.getQueue(presentFamily);
	if (presentQueue == VK_NULL_HANDLE){
		RAINDROP_log(ERROR, GRAPHICS, "failed to found a present queue");
		return VK_ERROR_UNKNOWN;
	}

	VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

	_currentFrame = (_currentFrame + 1) % _bufferCount;
	return result;
}

VkResult Swapchain::acquireNextImage(){
	RAINDROP_profile_function();
	auto& frame = _frames[_currentFrame];
	vkWaitForFences(_device.get(), 1, &frame._inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	VkResult result = vkAcquireNextImageKHR(_device.get(), _swapchain, std::numeric_limits<uint64_t>::max(), frame._imageAvailable, VK_NULL_HANDLE, &_currentFrame);
	return result;
}

maths::vec4<float32> Swapchain::clearColor() const{
	RAINDROP_profile_function();
	return _clearColor;
}

void Swapchain::setClearColor(maths::vec4<float32> color){
	RAINDROP_profile_function();
	_clearColor = color;
}

Swapchain::Frame::Frame(){
	_framebuffer = VK_NULL_HANDLE;
	_depth = VK_NULL_HANDLE;
	_image = VK_NULL_HANDLE;
	_imageMemory = VK_NULL_HANDLE;
	_depthMemory = VK_NULL_HANDLE;
	_imageView = VK_NULL_HANDLE;
	_depthView = VK_NULL_HANDLE;
	_imageAvailable = VK_NULL_HANDLE;
	_imageFinished = VK_NULL_HANDLE;
	_inFlightFence = VK_NULL_HANDLE;
	_imageInFlight = VK_NULL_HANDLE;
}