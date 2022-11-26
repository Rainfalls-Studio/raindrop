/**
 * @brief this file contain the vulkan memory buffer
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 */

#ifndef __RAINDROP_RENDER_VULKAN_TEXTURE_HPP__
#define __RAINDROP_RENDER_VULKAN_TEXTURE_HPP__

#include "Framebuffer.hpp"
#include "LogicalDevice.hpp"
#include <vulkan/vulkan.h>

#include <filesystem>

namespace rnd::render::vulkan{
	struct TextureBuilder{
		VkFilter magFilter = VK_FILTER_LINEAR;
		VkFilter minFilter = VK_FILTER_LINEAR;
		VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		bool anisotropy = false;
		bool normalizeCoords = false;

		VkImageTiling tiling = VK_IMAGE_TILING_LINEAR;
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
	};

	class Texture{
		public:
			static constexpr uint32_t DEPTH_ATTACHMENT = -1;

			Texture(LogicalDevice* device, Framebuffer &framebuffer, uint32_t attachment, TextureBuilder& builder);
			Texture(LogicalDevice* device, const std::filesystem::path &path, TextureBuilder& builder);
			Texture(LogicalDevice* device, VkFormat format, VkExtent2D extent, void* data, uint32_t pixelSize, TextureBuilder& builder);

			~Texture();

			Texture(const Texture &) = delete;
			Texture& operator=(const Texture &) = delete;

			VkDescriptorImageInfo getDescriptorInfo();

			VkImage getImage();
			VkDeviceMemory getImageMemory();
			VkImageView getImageView();
			VkSampler getSampler();

		private:
			void createFromData(void* data, uint32_t pixelSize, TextureBuilder& builedr);

			void createImageSampler(TextureBuilder& builder);
			void createImage(TextureBuilder& builder);
			void createImageView(TextureBuilder& builder);

			template<typename T>
			struct Member{
				Member<T>(const T &other) {member = other;}
				Member<T>& operator=(const T &other) {member = other; return *this;}
				operator T&() {return member;}
				T& operator*() {return member;}
				T* operator->() {return &member;}
				T* operator&() {return &member;}

				T member;
				bool custom = false;
			};

			LogicalDevice* device = nullptr;
			VkExtent2D extent;
			VkFormat format;
			VkImageLayout layout;

			Member<VkImage> image = VK_NULL_HANDLE;
			Member<VkDeviceMemory> imageMemory = VK_NULL_HANDLE;
			Member<VkImageView> imageView = VK_NULL_HANDLE;
			VkSampler imageSampler = VK_NULL_HANDLE;

			VkFormat channelCountToVkFormat(int channel, VkImageTiling tiling, VkFormatFeatureFlags features);
			
	};
}

#endif