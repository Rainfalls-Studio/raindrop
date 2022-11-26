#include "render/vulkan/SwapChain.hpp"
#include "core.hpp"

#include <vulkan/vk_enum_string_helper.h>
#include <limits>
#include <stdexcept>

namespace rnd::render::vulkan{
	SwapChain::SwapChain(std::shared_ptr<SwapChain> previous){
		PROFILE_FUNCTION();
		oldSwapChain = previous;
	}

	SwapChain::~SwapChain(){
		PROFILE_FUNCTION();
		if (swapchain){
			framebuffers.clear();

			vkDestroySwapchainKHR(device->getDevice(),swapchain, nullptr);
			vkDestroyRenderPass(device->getDevice(), renderPass, nullptr);

			for (size_t i=0; i<static_cast<size_t>(framesInFlight); i++) {
				vkDestroySemaphore(device->getDevice(), renderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(device->getDevice(), imageAvailableSemaphores[i], nullptr);
				vkDestroyFence(device->getDevice(), inFlightFences[i], nullptr);
			}
		}
	}
	
	void SwapChain::create(){
		PROFILE_FUNCTION();
		createSwapchain();
		createRenderPass();
		createFramebuffers();
		createSyncObjects();
		oldSwapChain = nullptr;
	}

	void SwapChain::init(SwapChainBuilder &builder){
		PROFILE_FUNCTION();
		extent = builder.extent;
		refreshType = builder.refreshMode;
		device = builder.device;
		
		framebuffers.clear();
		create();
	}

	VkFormat SwapChain::findDepthFormat(){
		PROFILE_FUNCTION();
		return device->getPhysicalDevice()->findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	void SwapChain::createSwapchain(){
		PROFILE_FUNCTION();
		PhysicalDevice* physicalDevice = device->getPhysicalDevice();
		auto swapChainSupport = physicalDevice->getSwapChainSupport();

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		framesInFlight = imageCount;
		

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface =  device->getInstance()->getSurface();
		
		// color buffer
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		const uint32_t graphicFamily = physicalDevice->getFamily(FAMILY_GRAPHIC).family;
		const uint32_t presentFamily = physicalDevice->getFamily(FAMILY_PRESENT).family;

		uint32_t queueFamilyIndices[] = {graphicFamily, presentFamily};

		if (queueFamilyIndices[0] != queueFamilyIndices[1]){
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = oldSwapChain == nullptr ? VK_NULL_HANDLE : oldSwapChain->swapchain;

		{
			VkResult result = vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapchain);

			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("create render swapchain : vkCreateSwapchainKHR :: ", string_VkResult(result));
			}
		}
		
		{
			VkResult result = vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, nullptr);
			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("render swapchain, failed to get the swapchain images : vkGetSwapchainImagesKHR :: ", string_VkResult(result));
			}
		}

		images.resize(imageCount);
		vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, images.data());

		imageFormat = surfaceFormat.format;
		extent = extent;
	}

	void SwapChain::createFramebuffers(){
		PROFILE_FUNCTION();
		depthFormat = findDepthFormat();

		framebuffers.resize(images.size());

		for (int i=0; i<images.size(); i++){
			FramebufferBuilder builder;
			builder.setExtent(extent);
			builder.addRenderPass(renderPass);
			builder.setLogicalDevice(device);

			FramebufferAttachments attachments = {
				{.format = imageFormat, .image = images[i]}
			};

			builder.enableDepthAttachment(depthFormat);
			builder.setAttachments(attachments);
			framebuffers[i].init(builder);
		}
	}

	void SwapChain::createSyncObjects(){
		PROFILE_FUNCTION();
		imageAvailableSemaphores.resize(framesInFlight);
		renderFinishedSemaphores.resize(framesInFlight);
		inFlightFences.resize(framesInFlight);
		imagesInFlight.resize(images.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < static_cast<size_t>(framesInFlight); i++) {
			if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS){

				RND_RUNTIME_ERR("render swaphain, failed to create synchronization objects");
			}
		}
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats){
		PROFILE_FUNCTION();
		for (const auto &availableFormat : availableFormats) {
			if (availableFormat.format == wantedSurfaceFormat.format && availableFormat.colorSpace == wantedSurfaceFormat.colorSpace) 
				return availableFormat;
		}

		fprintf(stderr, "WARNING :: failed to found the wanted swap chain surface format (format : %d color space : %d) set as the first available format \n", static_cast<int>(wantedSurfaceFormat.format), static_cast<int>(wantedSurfaceFormat.colorSpace));

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes){
		PROFILE_FUNCTION();
		for (const auto &presentMode : availablePresentModes){
			if (presentMode == static_cast<VkPresentModeKHR>(refreshType))
				return presentMode;
		}

		fprintf(stderr, "WARNING :: failed to found the wanted swap chain present mode : %d | set as V-sync VK_PRESENT_MODE_FIFO_KHR\n", refreshType);

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities){
		PROFILE_FUNCTION();
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		else {
			VkExtent2D actualExtent = windowExtent;
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	VkFramebuffer SwapChain::getFrameBuffer(uint8_t index){
		PROFILE_FUNCTION();
		return framebuffers[index].getFramebuffer();
	}

	VkImageView SwapChain::getImageView(uint8_t index){
		PROFILE_FUNCTION();
		return framebuffers[index].getAttachmentImageView(0);
	}

	VkRenderPass SwapChain::getRenderPass(){
		PROFILE_FUNCTION();
		return renderPass;
	}

	uint8_t SwapChain::getImageCount(){
		PROFILE_FUNCTION();
		return images.size();
	}

	VkFormat SwapChain::getImageFormat(){
		PROFILE_FUNCTION();
		return imageFormat;
	}
	
	VkExtent2D SwapChain::getExtent(){
		PROFILE_FUNCTION();
		return extent;
	}

	uint8_t SwapChain::getFrameInFlightCount(){
		PROFILE_FUNCTION();
		return framesInFlight;
	}

	VkResult SwapChain::acquireNextImage(uint32_t *imageIndex){
		PROFILE_FUNCTION();
		vkWaitForFences(device->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		VkResult result = vkAcquireNextImageKHR(device->getDevice(), swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);
		return result;
	}

	VkResult SwapChain::submitCommandBuffer(VkCommandBuffer* buffers, uint32_t *imageIndex){
		PROFILE_FUNCTION();

		{
			PROFILE_SCOPE("wait for fence :: vkWaitForFences");
			if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
				vkWaitForFences(device->getDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
		}
		
		imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffers;

		VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		{
			PROFILE_SCOPE("reset fences :: vkResetFences");
			VkResult result = vkResetFences(device->getDevice(), 1, &inFlightFences[currentFrame]);

			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("render swapchain, failed to reste a syncronisation fence : vkResetFences :: ", string_VkResult(result));
			}
		}

		{
			PROFILE_SCOPE("submit queue :: vkQueueSubmit");
			VkResult result = vkQueueSubmit(device->getQueue(QueueFamily::FAMILY_GRAPHIC, 0), 1, &submitInfo, inFlightFences[currentFrame]);

			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("render swapchain, failed to submit frame command buffer : vkQueueSubmit :: ", string_VkResult(result));
			}
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {swapchain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = imageIndex;

		
		VkResult result;
		
		{
			PROFILE_SCOPE("queue present :: vkQueuePresentKHR");
			result = vkQueuePresentKHR(device->getQueue(QueueFamily::FAMILY_PRESENT, 0), &presentInfo);
		}

		currentFrame = (currentFrame + 1) % framesInFlight;

		return result;
	}

	bool SwapChain::compareFormats(const SwapChain& swapChain){
		PROFILE_FUNCTION();
		return swapChain.depthFormat == depthFormat && swapChain.imageFormat == imageFormat;
	}

	SwapChainRefreshMode SwapChain::getRefreshMode(){
		PROFILE_FUNCTION();
		return refreshType;
	} 

	VkImage SwapChain::getCurrentImage(){
		PROFILE_FUNCTION();
		return framebuffers[currentFrame].getAttachmentImage(0);
	}

	void SwapChain::createRenderPass(){
		PROFILE_FUNCTION();
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 1;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments = {depthAttachment, colorAttachment};

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult result = vkCreateRenderPass(device->getDevice(), &renderPassInfo, nullptr, &renderPass);

		if (result != VK_SUCCESS){
			RND_RUNTIME_ERR("render swapchain, failed to create the render pass : vkCreateRenderPass :: ", string_VkResult(result));
		}
	}
}