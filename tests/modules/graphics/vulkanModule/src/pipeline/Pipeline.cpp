#include "pipeline/Pipeline.hpp"
#include "pipeline/Shader.hpp"

Pipeline::Pipeline(GfxAllocator& allocator, Device& device) : _allocator{allocator}, _device{device}{
	RAINDROP_profile_function();
	_pipeline = VK_NULL_HANDLE;
	_layout = VK_NULL_HANDLE;

	for (usize i=0; i<STAGE_BIT_COUNT; i++){
		_shaders[i] = nullptr;
	}

	createLayout();
}

Pipeline::~Pipeline(){
	RAINDROP_profile_function();
	if (_pipeline) vkDestroyPipeline(_device.get(), _pipeline, _allocator.callback());
	if (_layout) vkDestroyPipelineLayout(_device.get(), _layout, _allocator.callback());

	_layout = VK_NULL_HANDLE;
	_pipeline = VK_NULL_HANDLE;

	for (usize i=0; i<STAGE_BIT_COUNT; i++){
		_shaders[i] = nullptr;
	}
}

void Pipeline::pushShader(Shader* shader){
	RAINDROP_profile_function();

	usize id = Tools::getShiftOffset(shader->stage());
	_shaders[id] = reinterpret_cast<Shader*>(shader);
}

bool Pipeline::build(){
	RAINDROP_profile_function();
	return createPipeline();
}

bool Pipeline::createPipeline(){
	RAINDROP_profile_function();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = nullptr;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = nullptr;
	
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
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

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

	VkDynamicState dynamicStateEnables[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pDynamicStates = dynamicStateEnables;
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.flags = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 0;

	VkGraphicsPipelineCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.pVertexInputState = &vertexInputInfo;
	info.pViewportState = &viewportInfo;
	info.pInputAssemblyState = &inputAssemblyInfo;
	info.pRasterizationState = &rasterizationInfo;
	info.pMultisampleState = &multisampleInfo;
	info.pColorBlendState = &colorBlendInfo;
	info.pDynamicState = &dynamicStateInfo;
	info.pDepthStencilState = &depthStencilInfo;

	Array<VkPipelineShaderStageCreateInfo> stagesInfos(_allocator.stack(), shaderCount());
	populateStagesInfo(stagesInfos);

	info.stageCount = static_cast<uint32_t>(stagesInfos.size());
	info.pStages = stagesInfos.data();

	info.layout = _layout;
	info.subpass = VK_SUBPASS_EXTERNAL;
	info.renderPass = VK_NULL_HANDLE;

	VkResult result = vkCreateGraphicsPipelines(_device.get(), VK_NULL_HANDLE, 1, &info, _allocator.callback(), &_pipeline);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan graphics pipeline : %s", string_VkResult(result));
		return false;
	}
	return true;
}

bool Pipeline::createLayout(){
	RAINDROP_profile_function();
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pushConstantRangeCount = 0;
	info.setLayoutCount = 0;
	
	VkResult result = vkCreatePipelineLayout(_device.get(), &info, _allocator.callback(), &_layout);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan pipeline layout : %s", string_VkResult(result));
		return false;
	}
}

uint32 Pipeline::shaderCount(){
	RAINDROP_profile_function();

	uint32 count=0;
	for (usize i=0; i<STAGE_BIT_COUNT; i++){
		if (_shaders[i] != nullptr) count++;
	}
	return count;
}

void Pipeline::populateStagesInfo(Array<VkPipelineShaderStageCreateInfo>& infos){
	RAINDROP_profile_function();

	uint32 i=0;
	for (usize j=0; j<STAGE_BIT_COUNT; j++){
		auto& shader = _shaders[j];
		if (shader == nullptr) continue;

		infos[i] = shader->stageCreateInfo();
		i++;
	}
}

void Pipeline::setRenderPass(VkRenderPass renderPass){
	RAINDROP_profile_function();
	_renderPass = renderPass;
}
