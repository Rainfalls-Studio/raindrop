#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/Device.hpp>

namespace Raindrop::Graphics::Builders{
	PipelineBuilder::PipelineBuilder(){
	}

	PipelineBuilder::~PipelineBuilder(){
	}

	std::shared_ptr<GraphicsPipeline> PipelineBuilder::build(VkAllocationCallbacks* callbacks){
		CLOG(INFO, "Engine.Graphics") << "Building vulkan graphics pipeline";

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = nullptr;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = nullptr;
		viewportInfo.pNext = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.depthClampEnable = VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationInfo.lineWidth = 1.0f;
		rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationInfo.depthBiasEnable = VK_FALSE;
		rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleInfo.minSampleShading = 1.0f;           // Optional
		multisampleInfo.pSampleMask = nullptr;             // Optional
		multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable = VK_FALSE;
		colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		colorBlendInfo.attachmentCount = 1;
		colorBlendInfo.pAttachments = &colorBlendAttachment;
		colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = VK_TRUE;
		depthStencilInfo.depthWriteEnable = VK_TRUE;
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		depthStencilInfo.stencilTestEnable = VK_FALSE;
		depthStencilInfo.front = {};  // Optional
		depthStencilInfo.back = {};   // Optional

		std::vector<VkDynamicState> dynamicStateEnables{};
		dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};


		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
		dynamicStateInfo.flags = 0;

		VkRenderPass renderPass = VK_NULL_HANDLE;
		uint32_t subpass = 0;
		
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages(_shaders.size());

		for (int i=0; i<static_cast<int>(shaderStages.size()); i++){
			auto &stage = shaderStages[i];
			
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.module = _shaders[i]->get();
			stage.pName = "main";
			stage.flags = 0;
			stage.pNext = nullptr;
			stage.stage = _shaders[i]->stage();
			stage.pSpecializationInfo = nullptr;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		createInfo.pStages = shaderStages.data();

		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &inputAssemblyInfo;
		createInfo.pViewportState = &viewportInfo;
		createInfo.pRasterizationState = &rasterizationInfo;
		createInfo.pMultisampleState = &multisampleInfo;
		createInfo.pDepthStencilState = &depthStencilInfo;
		createInfo.pColorBlendState = &colorBlendInfo;
		createInfo.pDynamicState = &dynamicStateInfo;

		VkPipelineLayout layout = createPipelineLayout(callbacks);

		createInfo.layout = layout;
		createInfo.renderPass = _renderPass;
		createInfo.subpass = _subpass;

		createInfo.basePipelineIndex = -1;

		try{
			std::shared_ptr<GraphicsPipeline> pipeline = std::make_shared<GraphicsPipeline>(_device, createInfo, _shaders, callbacks);
			destroyPipelineLayout(layout, callbacks);
			return pipeline;
		} catch (const std::runtime_error& e){
			destroyPipelineLayout(layout, callbacks);
			throw e;
		}

		return 0;
	}

	void PipelineBuilder::addShader(const std::shared_ptr<Shader>& shader){
		_shaders.push_back(shader);
	}

	void PipelineBuilder::setDevice(const std::shared_ptr<Device>& device){
		_device = device;
	}

	void PipelineBuilder::setRenderPass(VkRenderPass renderPass){
		_renderPass = renderPass;
	}
	
	VkPipelineLayout PipelineBuilder::createPipelineLayout(VkAllocationCallbacks* callbacks){

		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.pushConstantRangeCount = 0;
		createInfo.setLayoutCount = 0;
		createInfo.flags = 0;
		
		VkPipelineLayout layout;
		if (vkCreatePipelineLayout(_device->get(), &createInfo, callbacks, &layout) != VK_SUCCESS){
			throw std::runtime_error("Failed to create vulkan pipeline layout");
		}

		return layout;
	}

	void PipelineBuilder::destroyPipelineLayout(VkPipelineLayout layout, VkAllocationCallbacks* callbacks){
		vkDestroyPipelineLayout(_device->get(), layout, callbacks);
	}
}