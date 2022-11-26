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

#ifndef __RAINDROP_RENDER_VULKAN_PIPELINE_HPP__
#define __RAINDROP_RENDER_VULKAN_PIPELINE_HPP__

#include "builders/PipelineBuilder.hpp"
#include "DescriptorSet.hpp"

namespace rnd::render::vulkan{
	class Pipeline{
		friend class PipelineLibrary;
		public:
			Pipeline() = default;
			Pipeline(PipelineBuilder& builder);
			~Pipeline();

			Pipeline(const Pipeline &) = delete;
			Pipeline& operator=(const Pipeline &) = delete;

			void init(PipelineBuilder& builder);
			const PipelineConfigInfo& getConfig();
			void bind(VkCommandBuffer commandBuffer, uint32_t setsIndex[]);
			void bindPushConstant(VkCommandBuffer, void* data); 

		private:
			struct ShaderModule{
				VkShaderModule shaderModule;
				VkShaderStageFlagBits stage;
			};

			void createGraphicPipeline(PipelineBuilder &builder);
			void createPipelineLayout(PipelineBuilder &builder);
			void createShaderModules(PipelineBuilder &builder);
			void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);

			LogicalDevice* device = nullptr;

			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout layout = VK_NULL_HANDLE;
			uint32_t pushConstantSize = 0;
			VkShaderStageFlags pushConstantStages;

			PipelineConfigInfo config;
			std::vector<ShaderModule> shaderModules;
			std::vector<DescriptorSet*> sets;
	};
}

#endif