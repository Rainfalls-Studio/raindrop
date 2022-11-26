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

#ifndef __RAINDROP_RENDER_VULKAN_DESCRIPTOR_SET_LAYOUT_HPP__
#define __RAINDROP_RENDER_VULKAN_DESCRIPTOR_SET_LAYOUT_HPP__

#include <vulkan/vulkan.h>
#include "builders/DescriptorSetLayoutBuilder.hpp"

namespace rnd::render::vulkan{
	class DescriptorSetLayout{
		friend class DescriptorWriter;
		public:
			DescriptorSetLayout() = default;
			DescriptorSetLayout(const DescriptorSetLayoutBuilder &builder){
				init(builder);
			}
			~DescriptorSetLayout();
			void init(const DescriptorSetLayoutBuilder &builder);

			VkDescriptorSetLayout getDescriptorSetLayout();

		private:
			LogicalDevice* device = nullptr;
			VkDescriptorSetLayout setLayout;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
	};
}

#endif