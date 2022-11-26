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

#ifndef __RAINDROP_RENDER_VULKAN_DESCRIPTOR_POOL_HPP__
#define __RAINDROP_RENDER_VULKAN_DESCRIPTOR_POOL_HPP__

#include "builders/DescriptorPoolBuilder.hpp"

namespace rnd::render::vulkan{
	class DescriptorPool{
		friend class DescriptorWriter;
		public:
			DescriptorPool() = default;
			DescriptorPool(DescriptorPoolBuilder &builder){
				init(builder);
			}

			~DescriptorPool();

			void init(DescriptorPoolBuilder &builder);

			bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor);
			void freeDescriptors(std::vector<VkDescriptorSet> &descriptors);
			void resetPool();
			VkDescriptorPool getPool();

		private:
			LogicalDevice* device = nullptr;
			VkDescriptorPool pool = VK_NULL_HANDLE;
	};
}

#endif