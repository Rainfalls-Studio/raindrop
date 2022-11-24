#include "render/vulkan/Pipeline.hpp"

#include "render/vulkan/DescriptorSetLayout.hpp"

#include <fstream>
#include <stdexcept>
#include <cassert>

namespace rnd::render::vulkan{

	std::vector<char> readFile(const std::string &filepath){
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::string("failed to open : " + filepath).c_str();
		
		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	Pipeline::Pipeline(PipelineBuilder& builder){
		initialize(builder);
	}

	Pipeline::~Pipeline(){
		if (pipeline){
			VkDevice device = this->device->getDevice();
			for (auto m : shaderModules){
				vkDestroyShaderModule(device, m.shaderModule, nullptr);
			}

			vkDestroyPipeline(device, pipeline, nullptr);
			vkDestroyPipelineLayout(device, layout, nullptr);
		}
	}

	void Pipeline::initialize(PipelineBuilder& builder){
		device = builder.device;

		createShaderModules(builder);
		createPipelineLayout(builder);
		createGraphicPipeline(builder);
	}

	const PipelineConfigInfo& Pipeline::getConfig(){
		return config;
	}

	void Pipeline::bind(VkCommandBuffer commandBuffer, uint32_t setsIndex[]){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		if (!sets.empty()){
			std::vector<VkDescriptorSet> descriptorSets(sets.size());
			for (uint32_t i=0; i<sets.size(); i++){
				descriptorSets[i] = sets[i]->getSet(setsIndex[i]);
			}

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, static_cast<uint32_t>(sets.size()), descriptorSets.data(), 0, nullptr);
		}
	}

	void Pipeline::bindPushConstant(VkCommandBuffer commandBuffer, void *data){
		vkCmdPushConstants(commandBuffer, layout, pushConstantStages, 0, pushConstantSize, data);
	}

	void Pipeline::createGraphicPipeline(PipelineBuilder &builder){
		auto &config = builder.config;
		
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages(shaderModules.size());

		for (int i=0; i<static_cast<int>(shaderStages.size()); i++){
			auto &stage = shaderStages[i];
			
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.module = shaderModules[i].shaderModule;
			stage.pName = "main";
			stage.flags = 0;
			stage.pNext = nullptr;
			stage.stage = shaderModules[i].stage;
			stage.pSpecializationInfo = nullptr;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		VkVertexInputAttributeDescription* attributes = nullptr;
		VkVertexInputBindingDescription* descriptions = nullptr;

		if (builder.hasInstanceDescription || builder.hasVertexDescription){
			// vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(builder.vertexDescription.attributeDescriptions.size());
			// vertexInputInfo.pVertexAttributeDescriptions = builder.vertexDescription.attributeDescriptions.data();

			// vertexInputInfo.vertexBindingDescriptionCount = 1;
			// vertexInputInfo.pVertexBindingDescriptions = &builder.vertexDescription.bindingDescription;
			uint32_t attributeCount = static_cast<uint32_t>(builder.instanceDescription.attributeDescriptions.size() + builder.vertexDescription.attributeDescriptions.size());
			attributes = new VkVertexInputAttributeDescription[attributeCount];

			{
				uint32_t i = 0;
				for (auto d : builder.instanceDescription.attributeDescriptions){
					attributes[i] = d;
					i++;
				}

				for (auto d : builder.vertexDescription.attributeDescriptions){
					attributes[i] = d;
					i++;
				}
			}

			vertexInputInfo.vertexAttributeDescriptionCount = attributeCount;
			vertexInputInfo.pVertexAttributeDescriptions = attributes;

			vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(builder.hasInstanceDescription) + static_cast<uint32_t>(builder.hasVertexDescription);
			descriptions = new VkVertexInputBindingDescription[vertexInputInfo.vertexBindingDescriptionCount];

			{
				uint32_t i = 0;
				if (builder.hasInstanceDescription){
					descriptions[i] = builder.instanceDescription.bindingDescription;
					i++;
				}

				if (builder.hasVertexDescription){
					descriptions[i] = builder.vertexDescription.bindingDescription;
				}
			}

			vertexInputInfo.pVertexBindingDescriptions = descriptions;
		} else {
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.pVertexAttributeDescriptions = nullptr;

			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.pVertexBindingDescriptions = nullptr;
		}
		

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		createInfo.pStages = shaderStages.data();

		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &config.inputAssemblyInfo;
		createInfo.pViewportState = &config.viewportInfo;
		createInfo.pRasterizationState = &config.rasterizationInfo;
		createInfo.pMultisampleState = &config.multisampleInfo;
		createInfo.pDepthStencilState = &config.depthStencilInfo;
		createInfo.pColorBlendState = &config.colorBlendInfo;
		createInfo.pDynamicState = &config.dynamicStateInfo;

		createInfo.layout = layout;
		createInfo.renderPass = config.renderPass;
		createInfo.subpass = 0;

		createInfo.basePipelineIndex = -1;

		VkResult result = vkCreateGraphicsPipelines(device->getDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);
		if (result != VK_SUCCESS){
			throw "failed to create graphic pipeline";
		}

		if (attributes != nullptr) delete[] attributes;
		if (descriptions != nullptr) delete[] descriptions;
	}

	void Pipeline::createPipelineLayout(PipelineBuilder &builder){

		VkPushConstantRange range = {};
		range.offset = 0;
		range.size = builder.pushConstant.size;
		range.stageFlags = builder.pushConstant.stages;

		pushConstantSize = builder.pushConstant.size;
		pushConstantStages = builder.pushConstant.stages;

		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (range.size > 0){
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &range;
		} else {
			createInfo.pushConstantRangeCount = 0;
			createInfo.pPushConstantRanges = nullptr;
		}

		std::vector<VkDescriptorSetLayout> layouts(builder.sets.size());
		for (int i=0; i<layouts.size(); i++){
			layouts[i] = builder.sets[i]->getLayout().getDescriptorSetLayout();
		}
		
		if (layouts.empty()){
			createInfo.setLayoutCount = 0;
			createInfo.pSetLayouts = nullptr;
		} else {
			createInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
			createInfo.pSetLayouts = layouts.data();

			sets.resize(builder.sets.size());
			for (size_t i=0; i<sets.size(); i++){
				sets[i] = builder.sets[i];
			}
		}

		createInfo.flags = 0;

		if (vkCreatePipelineLayout(device
		->getDevice(), &createInfo, nullptr, &layout) != VK_SUCCESS){
			throw "failed to create pipeline layout";
		}
	}

	static inline VkShaderStageFlagBits pipelineStageToVkStage(ShaderStage stage){
		switch (stage){
			case SHADER_STAGE_VERTEX: return VK_SHADER_STAGE_VERTEX_BIT;
			case SHADER_STAGE_TESSELLATION_CONTROL: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case SHADER_STAGE_TESSELLATION_EVALUATION: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case SHADER_STAGE_GEOMETRY: return VK_SHADER_STAGE_GEOMETRY_BIT;
			case SHADER_STAGE_FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case SHADER_STAGE_COMPUTE: return VK_SHADER_STAGE_COMPUTE_BIT;
			case SHADER_STAGE_ALL_GRAPHICS: return VK_SHADER_STAGE_ALL_GRAPHICS;
			case SHADER_STAGE_ALL: return VK_SHADER_STAGE_ALL;
			case SHADER_STAGE_RAYGEN: return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
			case SHADER_STAGE_ANY_HIT: return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
			case SHADER_STAGE_CLOSEST_HIT: return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
			case SHADER_STAGE_MISS: return VK_SHADER_STAGE_MISS_BIT_KHR;
			case SHADER_STAGE_INTERSECTION: return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
			case SHADER_STAGE_CALLABLE: return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
			case SHADER_STAGE_TASK: return VK_SHADER_STAGE_TASK_BIT_NV;
			case SHADER_STAGE_MESH: return VK_SHADER_STAGE_MESH_BIT_NV;
		}
		assert(false && "invalid shader stage");
		return static_cast<VkShaderStageFlagBits>(-1);
	}

	void Pipeline::createShaderModules(PipelineBuilder &builder){
		std::vector<PipelineBuilder::PipelineStage> shaderStages;

		for (int i=0; i<SHADER_STAGE_COUNT; i++){
			if (!builder.shaderStages[i].required) continue;
			shaderStages.push_back(builder.shaderStages[i]);
		}

		shaderModules.resize(shaderStages.size());

		for (int i=0; i<static_cast<int>(shaderModules.size()); i++){
			std::vector<char> code = readFile(shaderStages[i].path.string());
			shaderModules[i].stage = pipelineStageToVkStage(shaderStages[i].stage);
			createShaderModule(code, shaderModules[i].shaderModule);
		}
	}

	void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule){
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		createInfo.codeSize = static_cast<uint32_t>(code.size());

		if (vkCreateShaderModule(device->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
			throw "failed to create shader module";
		}
	}
}
