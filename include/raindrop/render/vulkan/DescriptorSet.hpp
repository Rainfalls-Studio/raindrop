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

#ifndef __RAINDROP_RENDER_VULKAN_DESCRIPTOR_SET_HPP__
#define __RAINDROP_RENDER_VULKAN_DESCRIPTOR_SET_HPP__

#include "builders/DescriptorSetBuilder.hpp"
#include <vulkan/vulkan.h>

#include "render/vulkan/Buffer.hpp"
#include "render/vulkan/DescriptorPool.hpp"
#include "render/vulkan/DescriptorSetLayout.hpp"

namespace rnd::render::vulkan{
	class DescriptorSet{
		public:
			DescriptorSet() = default;
			DescriptorSet(DescriptorSetBuilder &builder){
				init(builder);
			}

			~DescriptorSet();

			void init(DescriptorSetBuilder &builder);

			DescriptorSetLayout& getLayout();
			DescriptorPool& getPool();
			VkDescriptorSet* getSets();
			VkDescriptorSet getSet(uint32_t index);

			void* getBuffer(uint32_t setIndex, uint32_t binding);
			void writeBuffer(uint32_t setIndex, uint32_t binding, void* data);

			void setDescriptorImage(uint32_t setIndex, uint32_t binding, uint32_t index, VkDescriptorImageInfo info);
		
		private:
			void createPool(DescriptorSetBuilder &builder);
			void createSetLayout(DescriptorSetBuilder &builder);
			void createBuffers(DescriptorSetBuilder &builder);
			void createDescriptorSets(DescriptorSetBuilder &builder);

			struct BufferRange{
				uint32_t offset;
				uint32_t size;
				uint32_t alignement;
			};

			// store the builder for when the user want to overwrite a texture
			DescriptorSetBuilder builder;
			LogicalDevice* device = nullptr;

			DescriptorPool pool;
			DescriptorSetLayout layout;
			BufferRange* buffers = nullptr;
			Buffer* buffer = nullptr;
			VkDescriptorSet* sets = nullptr;
			uint32_t descriptorSetCount = 0;
	};
}

#endif