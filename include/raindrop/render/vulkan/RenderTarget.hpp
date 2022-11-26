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

#ifndef __RAINDROP_RENDER_VULKAN_RENDER_TARGET_HPP__
#define __RAINDROP_RENDER_VULKAN_RENDER_TARGET_HPP__
#include "builders/RenderTargetBuilder.hpp"
#include "Framebuffer.hpp"

#include <vulkan/vulkan.h>

namespace rnd::render::vulkan{
	class RenderTarget{
		public:
			using ClearColor = RenderTargetBuilder::ClearColor;

			RenderTarget() = default;
			RenderTarget(RenderTargetBuilder &builder){
				init(builder);
			}

			~RenderTarget();

			RenderTarget(const RenderTarget &) = delete;
			RenderTarget& operator=(const RenderTarget &) = delete;

			void init(RenderTargetBuilder &builder);
			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);
			VkRenderPass getRenderPass();
			Framebuffer* getFramebuffer();

			void resize(VkExtent2D extent);
		
		private:
			void createRenderPass(RenderTargetBuilder &builder);

			LogicalDevice* device = nullptr;
			std::vector<VkClearValue> clearValues;
			bool customFramebuffer = false;
			Framebuffer* framebuffer = nullptr;

			VkRenderPass renderPass = nullptr;
			VkViewport viewport;
			VkRect2D scissor;
	};
}

#endif