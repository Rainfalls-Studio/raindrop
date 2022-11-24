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

#ifndef __RAINDROP_RENDER_VULKAN_FRAME_BUFFER_HPP__
#define __RAINDROP_RENDER_VULKAN_FRAME_BUFFER_HPP__
#include "builders/FramebufferBuilder.hpp"

namespace rnd::render::vulkan{
	class Framebuffer{
		friend class RenderTarget;
		friend class Texture;
		public:
			Framebuffer() = default;
			Framebuffer(FramebufferBuilder &builder);
			~Framebuffer();

			void initialize(FramebufferBuilder &builder);
			void destroy();

			VkImage getAttachmentImage(uint8_t attachmentIndex);
			VkImageView getAttachmentImageView(uint8_t attachmentIndex);
			VkFormat getAttachmentFormat(uint8_t attachmentIndex);
			VkImage getDepthAttachmentImage();
			VkDeviceMemory getDepthAttachmentMemory();
			VkImageView getDepthAttachmentImageView();
			VkFormat getDepthAttachmentFormat();
			VkFramebuffer getFramebuffer();
			VkExtent2D getExtent();

			void resize(VkExtent2D extent);
		
		private:
			template<typename T>
			struct CustomableMember{
				CustomableMember<T>(const T &other) {member = other;}
				CustomableMember<T>& operator=(const T &other) {member = other; return *this;}
				operator T&() {return member;}
				T& operator*() {return member;}
				T* operator->() {return &member;}

				T member;
				bool custom = false;
			};

			struct ColorAttachment{
				VkFormat format;
				VkSampleCountFlagBits samples;
				VkImageTiling tiling;
				CustomableMember<VkImage> image = VK_NULL_HANDLE;
				CustomableMember<VkImageView> imageView = VK_NULL_HANDLE;
				VkDeviceMemory imageMemory;
				VkExtent2D maxExtent;
			};

			VkImage createColorAttachmentImage(FramebufferBuilder &builder, FramebufferAttachments::Attachment &attachmentBuilder, ColorAttachment& attachment);
			VkImageView createColorAttachmentImageView(FramebufferBuilder &builder, uint8_t attachmentIndex);

			VkImage createDepthAttachmentImage(FramebufferBuilder &builder);
			VkImageView createDepthAttachmentImageView(FramebufferBuilder &builder);

			VkRenderPass createRenderPass(FramebufferBuilder &builder);
			VkFramebuffer createFramebuffer(FramebufferBuilder &builder);


			LogicalDevice* device = nullptr;
			CustomableMember<VkRenderPass> renderPass = VK_NULL_HANDLE;
			VkFramebuffer framebuffer;
			VkExtent2D extent;

			std::vector<ColorAttachment> colorAttachments;

			bool depthAttachmentEnabled = false;
			VkFormat depthFormat;
			CustomableMember<VkImage> depthImage = VK_NULL_HANDLE;
			CustomableMember<VkImageView> depthImageView = VK_NULL_HANDLE;
			VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
	};
}

#endif