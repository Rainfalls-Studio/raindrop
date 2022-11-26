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

#ifndef __RAINDROP_RENDER_VULKAN_DESCRIPTOR_WRITER_HPP__
#define __RAINDROP_RENDER_VULKAN_DESCRIPTOR_WRITER_HPP__

#include "DescriptorSetLayout.hpp"
#include "DescriptorPool.hpp"
#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace rnd::render::vulkan{
	class DescriptorWriter{
		public:
			DescriptorWriter(LogicalDevice* device, DescriptorSetLayout* setLayout, DescriptorPool* pool){
				init(device, setLayout, pool);
			}

			void init(LogicalDevice* device, DescriptorSetLayout* setLayout, DescriptorPool* pool);
			
			void writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
			void writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
			void writeImages(uint32_t binding, uint32_t imageCount, VkDescriptorImageInfo *imageInfos);
			
			bool build(VkDescriptorSet &set);
			void overwrite(VkDescriptorSet &set);

		private:
			LogicalDevice* device = nullptr;
			DescriptorSetLayout* setLayout = nullptr;
			DescriptorPool* pool = nullptr;
			std::vector<VkWriteDescriptorSet> writes{};
	};
}

#endif