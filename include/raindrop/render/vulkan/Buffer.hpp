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

#ifndef __RAINDROP_RENDER_VULKAN_BUFFER_HPP__
#define __RAINDROP_RENDER_VULKAN_BUFFER_HPP__

#include <vulkan/vulkan.h>
#include "LogicalDevice.hpp"

namespace rnd::render::vulkan{
	class Buffer{
		public:
			Buffer() = default;
			~Buffer();

			// avoid copy
			Buffer(const Buffer &) = delete;
			Buffer &operator=(const Buffer &) = delete;

			void init(LogicalDevice* device);
			void alloc(VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryProperties);

			void setInstanceProperties(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignement);

			VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			void unmap();
			void write(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			void writeToIndex(void* data, int index);
			VkResult flushIndex(int index);
			VkDescriptorBufferInfo descriptorInfoForIndex(int index);
			VkResult invalidateIndex(int index);

			VkBuffer getBuffer() const;
			void* getMappedMemory() const;
			uint32_t getInstanceCount() const;
			VkDeviceSize getInstanceSize() const;
			VkDeviceSize getAlignmentSize() const;
			VkBufferUsageFlags getUsageFlags() const;
			VkMemoryPropertyFlags getMemoryPropertyFlags() const;
			VkDeviceSize getBufferSize() const;

		private:
			void destroy();

			LogicalDevice* device;

			uint32_t instanceCount;
			VkDeviceSize instanceSize;

			VkBuffer buffer = VK_NULL_HANDLE;
			VkDeviceMemory memory = VK_NULL_HANDLE;
			void* mapped = nullptr;

			VkDeviceSize bufferSize;
			VkDeviceSize alignmentSize;
			VkBufferUsageFlags usageFlags;
			VkMemoryPropertyFlags memoryPropertyFlags;
	};
}

#endif