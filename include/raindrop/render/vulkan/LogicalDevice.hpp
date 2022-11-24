/**
 * @brief this file contain the vulkan logical device
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

#ifndef __RAINDROP_RENDER_VULKAN_LOGICAL_DEVICE_HPP__
#define __RAINDROP_RENDER_VULKAN_LOGICAL_DEVICE_HPP__

#include "builders/LogicalDeviceBuilder.hpp"
#include <vulkan/vulkan.h>

#include <array>

namespace rnd::render::vulkan{
	class CommandPool; // create a prototype to avoid compilation error

	class LogicalDevice{
		public:
			LogicalDevice() = default;
			LogicalDevice(LogicalDeviceBuilder &builder){
				initialize(builder);
			}
			~LogicalDevice();

			LogicalDevice(const LogicalDevice &) = delete;
			LogicalDevice& operator=(const LogicalDevice &) = delete;

			void initialize(LogicalDeviceBuilder &builder);

			VkDevice getDevice() const;
			VkQueue getQueue(QueueFamily family, uint32_t index);
			PhysicalDevice* getPhysicalDevice();
			Instance* getInstance();

			void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory);
			CommandPool* getCommandPool(QueueFamily family);

		private:
			void createCommandPools(LogicalDeviceBuilder &builder);

			PhysicalDevice* physicalDevice = nullptr;
			VkDevice device = VK_NULL_HANDLE;

			struct QueueType{
				VkQueue* queues = nullptr;
				uint32_t queueCount = 0;
			};

			QueueType queues[FAMILY_COUNT];
			CommandPool* commandPools[FAMILY_COUNT];
	};
}

#endif