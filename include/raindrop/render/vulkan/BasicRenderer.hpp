/**
 * @brief this file contain the vulkan basic renderer.
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

#ifndef __RAINDROP_RENDER_VULKAN_BASIC_RENDERER_HPP__
#define __RAINDROP_RENDER_VULKAN_BASIC_RENDERER_HPP__

#include "core.hpp"
#include "builders/BasicRendererBuilder.hpp"
#include "CommandPool.hpp"
#include "utils.hpp"
#include "vulkan/vk_enum_string_helper.h"

namespace rnd::render::vulkan::renderer{
	class BasicRenderer{
		public:
			BasicRenderer(BasicRendererBuilder& builder){
				PROFILE_FUNCTION();
				init(builder);
			}

			BasicRenderer() = default;
			~BasicRenderer() = default;

			void init(BasicRendererBuilder& builder){
				PROFILE_FUNCTION();
				device = builder.device;
				extent = builder.extent;
				extent = builder.extent;
				refreshMode = builder.refreshMode;
				bufferCount = builder.bufferCount;
				clearColor = builder.clearColor;
				clearDepthStencil = builder.depthStencilClearValue;

				createSwapChain();
				createCommandBuffers();
			}

			void shutdown(){
				PROFILE_FUNCTION();
				vkDeviceWaitIdle(device->getDevice());
				swapChain.reset();

				vkFreeCommandBuffers(device->getDevice(), device->getCommandPool(FAMILY_GRAPHIC)->getCommandPool(), bufferCount, commandBuffers);
				delete[] commandBuffers;
			}

			void onWindowMinimized(){
				PROFILE_FUNCTION();
				extent = {0, 0};
				windowResized = true;
			}

			void onWindowResied(VkExtent2D extent){
				PROFILE_FUNCTION();
				this->extent = extent;

				// if the with or the height is null, do not render, otherwise render
				render = !(extent.width == 0 || extent.height == 0);
				windowResized = true;
			}

			void stopRendering(){
				PROFILE_FUNCTION();
				render = false;
			}

			void continueRendering(){
				PROFILE_FUNCTION();
				render = true;
			}
			
			// return the command buffer |!| VK_NULL_HANDLE if cannot render
			VkCommandBuffer beginFrame(){
				PROFILE_FUNCTION();
				if (!render) return VK_NULL_HANDLE;

				VkResult result = swapChain->acquireNextImage(&currentBufferIndex);
				if (windowResized || result == VK_ERROR_OUT_OF_DATE_KHR){
					createSwapChain();
					return VK_NULL_HANDLE;
				}


				if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        			throw "failed to acquire swap chain image";
				}

				frameStarted = true;

				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];

				VkCommandBufferBeginInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				
				result = vkBeginCommandBuffer(commandBuffer, &info);
				if (result != VK_SUCCESS){
					throw (std::string("failed to begin command buffer | ") + resultToStr(result)).c_str();
				}

				return commandBuffer;
			}

			void endFrame(){
				PROFILE_FUNCTION();
				RND_ASSERT(frameStarted == true, "cannot end a frame if it's not started");
				
				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];
				VkResult result = vkEndCommandBuffer(commandBuffer);

				if (result != VK_SUCCESS){
					throw (std::string("failed to end the command buffer | ") + resultToStr(result)).c_str();
				}
				
				result = swapChain->submitCommandBuffer(&commandBuffer, &currentBufferIndex);

				if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowResized){
					createSwapChain();
				} else if (result != VK_SUCCESS) {
					RND_RUNTIME_ERR("renderer, failed to submit the command buffer : swapchain::submitCommandBuffer :: ", string_VkResult(result));
				}

				frameStarted = false;
				currentBufferIndex = (currentBufferIndex + 1) % bufferCount;
			}

			void beginSwapChainRenderPass(){
				PROFILE_FUNCTION();
				assert(frameStarted == true && "cannot begin swap chain render pass if a fram is not a started");

				VkCommandBuffer commandBuffer = commandBuffers[currentBufferIndex];

				VkRenderPassBeginInfo info{};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				info.renderPass = swapChain->getRenderPass();
				info.framebuffer = swapChain->getFrameBuffer(currentBufferIndex);

				info.renderArea.offset = {0, 0};
				info.renderArea.extent = extent;

				VkClearValue clearValues[2];

				clearValues[0].depthStencil = clearDepthStencil;
				clearValues[1].color = clearColor;

				info.clearValueCount = 2;
				info.pClearValues = clearValues;

				vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

				VkViewport viewport{};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = static_cast<float>(extent.width);
				viewport.height = static_cast<float>(extent.height);
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				VkRect2D scissor{{0, 0}, extent};

				vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
			}

			void endSwapChainRenderPass(VkCommandBuffer commandBuffer){
				PROFILE_FUNCTION();
    			vkCmdEndRenderPass(commandBuffer);
			}

			void setClearColor(VkClearColorValue color){
				PROFILE_FUNCTION();
				clearColor = color;
			}

			void setClearDepthStencil(VkClearDepthStencilValue value){
				PROFILE_FUNCTION();
				clearDepthStencil = value;
			}

			VkRenderPass getSwapChainRenderPass(){
				PROFILE_FUNCTION();
				return swapChain->getRenderPass();
			}

		private:
			void createCommandBuffers(){
				PROFILE_FUNCTION();
				commandBuffers = new VkCommandBuffer[bufferCount];

				VkCommandBufferAllocateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				info.commandBufferCount = bufferCount;
				info.commandPool = device->getCommandPool(QueueFamily::FAMILY_GRAPHIC)->getCommandPool();
				info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				

				VkResult result = vkAllocateCommandBuffers(device->getDevice(), &info, commandBuffers);

				if (result != VK_SUCCESS){
					RND_RUNTIME_ERR("renderer, failed to callocate the command buffers : vkAllocateCommandBuffers :: ", string_VkResult(result));
				}
			}

			void createSwapChain(){
				PROFILE_FUNCTION();
				vkDeviceWaitIdle(device->getDevice());

				SwapChainBuilder scBuilder;
				scBuilder.setDevice(device);
				scBuilder.setExtent(extent);
				scBuilder.setSwapchainBufferCount(bufferCount);
				scBuilder.setRefreshMode(refreshMode);

				if (swapChain == nullptr){
					swapChain = std::make_shared<SwapChain>(scBuilder);
				} else {
					oldSwapChain = swapChain;
					swapChain = std::make_shared<SwapChain>(oldSwapChain);

					swapChain->init(scBuilder);

					if (!swapChain->compareFormats(*oldSwapChain)){
						RND_RUNTIME_ERR("renderer, failed to recreate the swapchain, the image or depth format has changed");
					}
				}

				windowResized = false;
			}

			LogicalDevice* device = nullptr;
			std::shared_ptr<SwapChain> swapChain = nullptr;
			std::shared_ptr<SwapChain> oldSwapChain = nullptr;
			VkCommandBuffer* commandBuffers = nullptr;

			uint32_t bufferCount = 0;
			uint32_t currentBufferIndex = 0;

			bool frameStarted = false;
			bool render = true;
			bool windowResized = false;

			VkExtent2D extent;
			SwapChainRefreshMode refreshMode;
			VkClearColorValue clearColor;
			VkClearDepthStencilValue clearDepthStencil;
	};
}

#endif