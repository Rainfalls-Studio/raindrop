#include "render/vulkan/Texture.hpp"
#include "render/vulkan/Buffer.hpp"
#include "render/vulkan/SingleTimeCommand.hpp"
#include "render/vulkan/utils.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace rnd::render::vulkan{
	Texture::Texture(LogicalDevice* device, Framebuffer &framebuffer, uint32_t attachment, TextureBuilder& builder){
		assert(device != nullptr && "cannot create a texture without a valid device");
		this->device = device;

		image.custom = true;
		imageView.custom = true;
		imageMemory.custom = true;
		extent = framebuffer.extent;

		if (attachment == DEPTH_ATTACHMENT){
			assert(framebuffer.depthAttachmentEnabled && "cannot use the the depth of a framebuffer that don't have one");

			image = framebuffer.depthImage;
			imageView = framebuffer.depthImageView;
			imageMemory = framebuffer.depthImageMemory;
			format = framebuffer.depthFormat;
			layout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;

		} else {
			auto &a = framebuffer.colorAttachments[attachment];
			imageView = a.imageView;
			imageMemory = a.imageMemory;
			format = a.format;
			image = a.image;
			layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}
		createImageSampler(builder);
	}


	// this format is supposed to work in any scenario
	#define DEFAULT_FORMAT VK_FORMAT_R8G8B8A8_SRGB
	#define DEFAULT_FORMAT_SIZE 4

	// if the first format isn't supported it will test the second one and go on... unit the default format
	#define R_FORMATS {VK_FORMAT_R8_SRGB, VK_FORMAT_R8_SNORM, VK_FORMAT_R8_UNORM, DEFAULT_FORMAT}
	#define RG_FORMATS {VK_FORMAT_R8G8_SRGB, VK_FORMAT_R8G8_SNORM, VK_FORMAT_R8G8_UNORM, DEFAULT_FORMAT}
	#define RGB_FORMATS {VK_FORMAT_R8G8B8_SRGB, VK_FORMAT_R8G8B8_SNORM, VK_FORMAT_R8G8B8_UNORM, DEFAULT_FORMAT}
	#define RGBA_FORMATS {VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_R8G8B8A8_SNORM, VK_FORMAT_R8G8B8A8_UNORM, DEFAULT_FORMAT}

	#define CHECK_FORMAT(format, channelNames, channelCount) for (auto f : channelNames ## _FORMATS){if (format == f) return channelCount;} 

	static inline int getChannelCount(VkFormat format){
		if (format == DEFAULT_FORMAT){
			return DEFAULT_FORMAT_SIZE;
		}

		CHECK_FORMAT(format, R, 1);
		CHECK_FORMAT(format, RG, 2);
		CHECK_FORMAT(format, RGB, 3);
		CHECK_FORMAT(format, RGBA, 4);

		return DEFAULT_FORMAT_SIZE;
	}

	VkFormat Texture::channelCountToVkFormat(int channel, VkImageTiling tiling, VkFormatFeatureFlags features){
		switch (channel){
			case 1: return device->getPhysicalDevice()->findSupportedFormat(R_FORMATS, tiling, features);
			case 2: return device->getPhysicalDevice()->findSupportedFormat(RG_FORMATS, tiling, features);
			case 3: return device->getPhysicalDevice()->findSupportedFormat(RGB_FORMATS, tiling, features);
			case 4: return device->getPhysicalDevice()->findSupportedFormat(RGBA_FORMATS, tiling, features);
		}
		assert(false && "cannot found a supported texture format");
		return VK_FORMAT_R8_UINT;
	}

	Texture::Texture(LogicalDevice* device, const std::filesystem::path &path, TextureBuilder& builder){
		assert(device != nullptr && "cannot create a texture without a valid device");
		this->device = device;

		int w, h, channel;
		void* pixels = stbi_load(path.string().c_str(), &w, &h, &channel, STBI_default);

		extent.width = static_cast<uint32_t>(w);
		extent.height = static_cast<uint32_t>(h);

		format = channelCountToVkFormat(channel, builder.tiling, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
		int supportedChannel = getChannelCount(format);

		if (supportedChannel != channel){
			unsigned char* oldPixels = static_cast<unsigned char*>(pixels);
			pixels = stbi__convert_format(oldPixels, channel, supportedChannel, w, h);
			channel = supportedChannel;
		}

		createFromData(pixels, channel, builder);
		stbi_image_free(pixels);
	}

	Texture::Texture(LogicalDevice* device, VkFormat format, VkExtent2D extent, void* data, uint32_t pixelSize, TextureBuilder& builder){
		assert(device != nullptr && "cannot create a texture without a valid device");
		this->device = device;
		
		this->extent = extent;
		this->format = format;

		createFromData(data, pixelSize, builder);
	}

	Texture::~Texture(){
		VkDevice device = this->device->getDevice();
		if (!image.custom) vkDestroyImage(device, image, nullptr);
		if (!imageView.custom) vkDestroyImageView(device, imageView, nullptr);
		if (!imageMemory.custom) vkFreeMemory(device, imageMemory, nullptr);
		vkDestroySampler(device, imageSampler, nullptr);
	}

	void Texture::createImageSampler(TextureBuilder& builder){
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = builder.magFilter;
		samplerInfo.minFilter = builder.minFilter;

		samplerInfo.addressModeU = builder.addressModeU;
		samplerInfo.addressModeV = builder.addressModeV;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		
		samplerInfo.anisotropyEnable = builder.anisotropy;
		samplerInfo.maxAnisotropy = device->getPhysicalDevice()->getProperties().limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = !builder.normalizeCoords;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		if (vkCreateSampler(device->getDevice(), &samplerInfo, nullptr, &imageSampler) != VK_SUCCESS)
			throw "failed to create sampler";
	}

	void Texture::createImage(TextureBuilder& builder){
		this->extent = extent;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.extent.width = extent.width;
		createInfo.extent.height = extent.height;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.format = format;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.usage =  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.samples = builder.samples;
		createInfo.flags = 0;
		createInfo.queueFamilyIndexCount = 1;

		uint32_t queueIndex = device->getPhysicalDevice()->getFamily(QueueFamily::FAMILY_GRAPHIC).family;
		createInfo.pQueueFamilyIndices = &queueIndex;

		device->createImageWithInfo(createInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image, imageMemory);
	}

	void Texture::createImageView(TextureBuilder& builder){
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.format = format;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device->getDevice(), &createInfo, nullptr, &imageView) != VK_SUCCESS){
			throw "failed to create image view";
		}
	}

	void Texture::createFromData(void* data, uint32_t pixelSize, TextureBuilder& builder){
		createImage(builder);
		createImageView(builder);

		Buffer staginBuffer;
		staginBuffer.initialize(device);

		VkDeviceSize imageSize = extent.width * extent.height * pixelSize;
		staginBuffer.alloc(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		staginBuffer.map();
		staginBuffer.write(data);
		staginBuffer.flush();
		staginBuffer.unmap();

		SingleTimeCommand command(device, QueueFamily::FAMILY_GRAPHIC, 0);
		transitionImageLayout(command.getCommandBuffer(), image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(command.getCommandBuffer(), staginBuffer.getBuffer(), image, extent.width, extent.height);
		transitionImageLayout(command.getCommandBuffer(), image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		command.end();

		createImageSampler(builder);
	}

	VkDescriptorImageInfo Texture::getDescriptorInfo(){
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = layout;
		imageInfo.imageView = imageView;
		imageInfo.sampler = imageSampler;
		return imageInfo;
	}

	VkImage Texture::getImage(){
		return image;
	}

	VkDeviceMemory Texture::getImageMemory(){
		return imageMemory;
	}

	VkImageView Texture::getImageView(){
		return imageView;
	}

	VkSampler Texture::getSampler(){
		return imageSampler;
	}
}