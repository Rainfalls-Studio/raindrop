#include <Raindrop/Graphics/WorldFramebuffer.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>

namespace Raindrop::Graphics{
	struct AttachmentInfo{
		VkAttachmentDescription description;
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;

		// if none of the formats are available, throw an exception
		std::vector<VkFormat> formats; // ordered from best to worst case
		VkFormatFeatureFlags requiredFeatures;
		VkClearValue clear;
	};
	
	// DEFAULT CONFIGURATION
	static std::vector<AttachmentInfo> attachments = {

		// DEPTH
		{
			.description = VkAttachmentDescription{	
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D24_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM_S8_UINT,
			},

			.requiredFeatures = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.depthStencil = VkClearDepthStencilValue{
					.depth = 1.f,
					.stencil = 0,
				}
			},
		},

		// ALBEDO [RGB] + SPECULAR [A]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_UINT,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_FORMAT_A8B8G8R8_UINT_PACK32,
				VK_FORMAT_R8G8B8A8_SRGB,
			},

			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// BLOOM [RGB]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
			
			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// NORMAL [XYZ]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R16G16B16A16_SNORM,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// POSITION [XYZ]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},
	};

	WorldFramebuffer::WorldFramebuffer(GraphicsContext& context, uint32_t width, uint32_t height) : _context{context}, _width{width}, _height{height}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.WorldFramebuffer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.WorldFramebuffer") << "Creating world framebuffer...";

		checkDefaultFormats();
		createRenderPass();
		createAttachments();
		createFramebuffer();

		createDescriptorPool();
		createSetLayout();
		createDescriptorSet();
		createPipeline();

		_context.gRegistry["WorldFramebuffer"] = this;

		CLOG(INFO, "Engine.Graphics.WorldFramebuffer") << "Created world framebuffer with success !";
	}

	WorldFramebuffer::~WorldFramebuffer(){
		if (_framebuffer) vkDestroyFramebuffer(_context.device.get(), _framebuffer, _context.allocationCallbacks);
		
		for (auto &a : _attachments){
			if (a.imageView) vkDestroyImageView(_context.device.get(), a.imageView, _context.allocationCallbacks);
			if (a.image) vkDestroyImage(_context.device.get(), a.image, _context.allocationCallbacks);
			if (a.memory) vkFreeMemory(_context.device.get(), a.memory, _context.allocationCallbacks);
			if (a.sampler) vkDestroySampler(_context.device.get(), a.sampler, _context.allocationCallbacks);
		}

		vkResetDescriptorPool(_context.device.get(), _descriptorPool->get(), 0);
		_descriptorPool.reset();
		_setLayout.reset();

		if (_renderPass) vkDestroyRenderPass(_context.device.get(), _renderPass, _context.allocationCallbacks);
	}

	void WorldFramebuffer::createRenderPass(){
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		std::vector<VkAttachmentReference> colorAttachmentRefs(attachments.size() - 1);
		for (int i=1; i<attachments.size(); i++){
			auto& ref = colorAttachmentRefs[i-1];
			ref.attachment = i;
			ref.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> descriptions(attachments.size());
		for (int i=0; i<attachments.size(); i++){
			descriptions[i] = attachments[i].description;
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(descriptions.size());
		renderPassInfo.pAttachments = descriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_context.device.get(), &renderPassInfo, _context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer render pass";
			throw std::runtime_error("Failed to create World framebuffer render pass");
		}
	}

	void WorldFramebuffer::createAttachments(){
		_attachments.resize(attachments.size());

		for (int i=0; i<attachments.size(); i++){
			auto& attachment = _attachments[i];
			VkImageCreateInfo imageInfo{};

			imageInfo = attachments[i].image;
			imageInfo.extent.width = _width;
			imageInfo.extent.height = _height;

			uint32_t familyIndices[] = {_context.graphicsFamily};

			imageInfo.pQueueFamilyIndices = familyIndices;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.queueFamilyIndexCount = sizeof(familyIndices) / sizeof(uint32_t);
			
			if (vkCreateImage(_context.device.get(), &imageInfo, _context.allocationCallbacks, &attachment.image) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") image";
				throw std::runtime_error("Failed to create world framebuffer attachment image");
			}

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(_context.device.get(), attachment.image, &requirements);

			VkMemoryAllocateInfo allocationInfo{};
			allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			allocationInfo.allocationSize = requirements.size;
			

			if (vkAllocateMemory(_context.device.get(), &allocationInfo, _context.allocationCallbacks, &attachment.memory) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to allocate world framebuffer attachment memory";
				throw std::runtime_error("Failed to allocate world framebuffer attachment memory");
			}

			if (vkBindImageMemory(_context.device.get(), attachment.image, attachment.memory, 0) != VK_SUCCESS){
				CLOG(ERROR, "Engine.graphics.WorldFramebuffer") << "Failed to bind world framebuffer attachment image memory";
				throw std::runtime_error("Failed to bind world framebuffer attachment image memory");
			}

			VkImageViewCreateInfo imageViewInfo{};
			imageViewInfo = attachments[i].imageView;
			imageViewInfo.image = attachment.image;

			if (vkCreateImageView(_context.device.get(), &imageViewInfo, _context.allocationCallbacks, &attachment.imageView) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") image view";
				throw std::runtime_error("Failed to create world framebuffer attachment image view");
			}

			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;

			if (vkCreateSampler(_context.device.get(), &samplerInfo, _context.allocationCallbacks, &attachment.sampler) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") sampler";
				throw std::runtime_error("Failed to create sampler");
			}
		}
	}

	void WorldFramebuffer::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = _width;
		info.height = _height;
		info.layers = 1;

		std::vector<VkImageView> imageViews(attachments.size());
		for (int i=0; i<attachments.size(); i++){
			imageViews[i] = _attachments[i].imageView;
		}

		info.pAttachments = imageViews.data();
		info.attachmentCount = imageViews.size();
		info.renderPass = _renderPass;
		
		if (vkCreateFramebuffer(_context.device.get(), &info, _context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer";
			throw std::runtime_error("Failed to create world framebuffer");
		}
	}

	void WorldFramebuffer::checkDefaultFormats(){
		for (auto &a : attachments){

			VkFormat bestCase = VK_FORMAT_UNDEFINED;

			if (bestCase == VK_FORMAT_UNDEFINED){
				for (auto &f : a.formats){
					VkFormatProperties properties;
					vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

					if (properties.optimalTilingFeatures & a.requiredFeatures){
						a.image.tiling = VK_IMAGE_TILING_OPTIMAL;
						bestCase = f;
						break;
					} else if (properties.linearTilingFeatures & a.requiredFeatures){
						a.image.tiling = VK_IMAGE_TILING_LINEAR;
						bestCase = f;
						break;
					}
				}
			}
			
			for (auto &f : a.formats){
				VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

				if (a.image.tiling == VK_IMAGE_TILING_OPTIMAL){
					if (properties.optimalTilingFeatures & a.requiredFeatures){
						bestCase = f;
						break;
					}
				} else if (a.image.tiling == VK_IMAGE_TILING_LINEAR){
					if (properties.linearTilingFeatures & a.requiredFeatures){
						bestCase = f;
						break;
					}
				}
			}


			if (bestCase == VK_FORMAT_UNDEFINED){
				CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "failed to find a format for world framebuffer attachment";
				throw std::runtime_error("failed to find a format for world framebuffer attachment");
			}

			a.imageView.format = bestCase;
			a.description.format = bestCase;
			a.image.format = bestCase;
		}
	}
	
	void WorldFramebuffer::beginRenderPass(VkCommandBuffer commandBuffer){
		VkRenderPassBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _renderPass;
		info.renderArea = VkRect2D{0, 0, _width, _height};

		info.clearValueCount = static_cast<uint32_t>(attachments.size());
		std::vector<VkClearValue> clear(attachments.size());

		for (int i=0; i<attachments.size(); i++){
			clear[i] = attachments[i].clear;
		}

		info.pClearValues = clear.data();

		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void WorldFramebuffer::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}
	
	WorldFramebuffer::Attachment& WorldFramebuffer::getAttachment(uint32_t id){
		return _attachments[id];
	}

	std::vector<WorldFramebuffer::Attachment>& WorldFramebuffer::getAttachments(){
		return _attachments;
	}

	VkRenderPass WorldFramebuffer::renderPass() const{
		return _renderPass;
	}

	uint32_t WorldFramebuffer::attachmentCount() const{
		return _attachments.size() - 1;
	}

	void WorldFramebuffer::createDescriptorPool(){
		Builders::DescriptorPoolBuilder builder;
		builder.setMaxSets(1);
		builder.pushPoolSize({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 5});

		_descriptorPool = builder.build(_context);
	}

	void WorldFramebuffer::createSetLayout(){
		Builders::DescriptorSetLayoutBuilder builder;

		for (int i=0; i<_attachments.size(); i++){
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = i;
			binding.descriptorCount = 1;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			binding.pImmutableSamplers = &_attachments[i].sampler;
			builder.pushBinding(binding);
		}

		_setLayout = builder.build(_context);
	}

	void WorldFramebuffer::createDescriptorSet(){
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = _descriptorPool->get();
		
		VkDescriptorSetLayout layout = _setLayout->get();
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(_context.device.get(), &info, &_descriptorSet) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer descriptor set";
			throw std::runtime_error("Failed to create world framebuffer descriptor set");
		}

		VkWriteDescriptorSet writes[5];
		VkDescriptorImageInfo infos[5];

		for (int i=0; i<sizeof(writes)/sizeof(VkWriteDescriptorSet); i++){
			infos[i] = {};
			writes[i] = {};

			infos[i].imageLayout = attachments[i].description.finalLayout;
			infos[i].imageView = _attachments[i].imageView;
			infos[i].sampler = _attachments[i].sampler;

			writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writes[i].dstBinding = i;
			writes[i].pImageInfo = &infos[i];
			writes[i].descriptorCount = 1;
			writes[i].dstSet = _descriptorSet;
		}

		vkUpdateDescriptorSets(_context.device.get(), sizeof(writes)/sizeof(VkWriteDescriptorSet), writes, 0, nullptr);
	}

	void WorldFramebuffer::createPipeline(){
		Builders::GraphicsPipelineBuilder builder;
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/worldFramebuffer/default.glsl.frag.spv").lock()));
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/worldFramebuffer/default.glsl.vert.spv").lock()));

		builder.setName("world framebuffer");
		builder.setRenderPass(_context.swapchain.renderPass());

		builder.addDescriptorSetLayout(_setLayout->get());
		builder.setAttachmentCount(1);

		builder.setVertexAttribtes({});
		builder.setVertexBindings({});

		_pipeline = builder.build(_context);
	}

	void WorldFramebuffer::render(VkCommandBuffer commandBuffer){
		_pipeline->bind(commandBuffer);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->layout(), 0, 1, &_descriptorSet, 0, nullptr);

		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}
}