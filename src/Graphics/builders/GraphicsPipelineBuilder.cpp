#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Renderer.hpp>

namespace Raindrop::Graphics::Builders{
	GraphicsPipelineBuilder::GraphicsPipelineBuilder(){
		_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		_viewportInfo.viewportCount = 1;
		_viewportInfo.pViewports = nullptr;
		_viewportInfo.scissorCount = 1;
		_viewportInfo.pScissors = nullptr;
		_viewportInfo.pNext = nullptr;

		_rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		_rasterizationInfo.depthClampEnable = VK_FALSE;
		_rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		_rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		_rasterizationInfo.lineWidth = 1.0f;
		_rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		_rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		_rasterizationInfo.depthBiasEnable = VK_FALSE;
		_rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		_rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		_rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		_multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_multisampleInfo.sampleShadingEnable = VK_FALSE;
		_multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		_multisampleInfo.minSampleShading = 1.0f;           // Optional
		_multisampleInfo.pSampleMask = nullptr;             // Optional
		_multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		_multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		_inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		_inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		_inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		_colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		_colorBlendInfo.logicOpEnable = VK_TRUE;
		_colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		_colorBlendInfo.attachmentCount = 0;
		_colorBlendInfo.pAttachments = nullptr;
		_colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		_colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		_colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		_colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		_depthStencilInfo.depthTestEnable = VK_TRUE;
		_depthStencilInfo.depthWriteEnable = VK_TRUE;
		_depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		_depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		_depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		_depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		_depthStencilInfo.stencilTestEnable = VK_FALSE;
		_depthStencilInfo.front = {};  // Optional
		_depthStencilInfo.back = {};   // Optional

		_tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		_tessellationInfo.patchControlPoints = 0;

		_vertexAttributes = {
			VkVertexInputAttributeDescription{
				.location = 0,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, position),
			},

			VkVertexInputAttributeDescription{
				.location = 1,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, color),
			},

			VkVertexInputAttributeDescription{
				.location = 2,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, normal),
			},

			VkVertexInputAttributeDescription{
				.location = 3,
				.binding = 0,
				.format = VK_FORMAT_R32G32_SFLOAT,
				.offset = offsetof(Vertex, uv),
			},
		};

		_vertexBindings = {
			VkVertexInputBindingDescription{
				.binding = 0,
				.stride = sizeof(Vertex),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			},
		};
	}

	GraphicsPipelineBuilder::~GraphicsPipelineBuilder(){
	}

	std::shared_ptr<GraphicsPipeline> GraphicsPipelineBuilder::build(GraphicsContext& context){
		CLOG(INFO, "Engine.Graphics") << "Building vulkan graphics pipeline";

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

		_colorBlendInfo.attachmentCount = static_cast<uint32_t>(_colorAttachments.size());
		_colorBlendInfo.pAttachments = _colorAttachments.data();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_vertexBindings.size());
		vertexInputInfo.pVertexBindingDescriptions = _vertexBindings.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_vertexAttributes.size());
		vertexInputInfo.pVertexAttributeDescriptions = _vertexAttributes.data();

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		createInfo.pStages = shaderStages.data();

		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &_inputAssemblyInfo;
		createInfo.pViewportState = &_viewportInfo;
		createInfo.pRasterizationState = &_rasterizationInfo;
		createInfo.pMultisampleState = &_multisampleInfo;
		createInfo.pDepthStencilState = &_depthStencilInfo;
		createInfo.pColorBlendState = &_colorBlendInfo;
		createInfo.pDynamicState = &dynamicStateInfo;
		createInfo.pTessellationState = &_tessellationInfo;

		createInfo.layout = VK_NULL_HANDLE; // Set by the pipeline itself
		createInfo.renderPass = _renderPass;
		createInfo.subpass = _subpass;
		createInfo.basePipelineIndex = -1;

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstant);
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		
		layoutInfo.pPushConstantRanges = &pushConstantRange;
		layoutInfo.pushConstantRangeCount = 1;

		layoutInfo.pSetLayouts = _setLayouts.data();
		layoutInfo.setLayoutCount = static_cast<uint32_t>(_setLayouts.size());
		layoutInfo.flags = 0;

		return std::make_shared<GraphicsPipeline>(context, createInfo, layoutInfo, _shaders, _name);
	}

	void GraphicsPipelineBuilder::addShader(const std::shared_ptr<Shader>& shader){
		_shaders.push_back(shader);
	}

	void GraphicsPipelineBuilder::addDescriptorSetLayout(const VkDescriptorSetLayout &layout){
		_setLayouts.push_back(layout);
	}

	void GraphicsPipelineBuilder::setRenderPass(VkRenderPass renderPass){
		_renderPass = renderPass;
	}

	void GraphicsPipelineBuilder::setAttachmentCount(uint32_t count){
		_colorAttachments.resize(count);

		for (auto& a : _colorAttachments){
			a.blendEnable = VK_TRUE;
			a.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			a.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			a.colorBlendOp = VK_BLEND_OP_ADD;
			a.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			a.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			a.alphaBlendOp = VK_BLEND_OP_ADD;
			a.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		}
	}

	void GraphicsPipelineBuilder::setName(const std::string& name){
		_name = name;
	}

	VkPipelineViewportStateCreateInfo& GraphicsPipelineBuilder::viewportInfo(){
		return _viewportInfo;
	}

	VkPipelineRasterizationStateCreateInfo& GraphicsPipelineBuilder::rasterizationInfo(){
		return _rasterizationInfo;
	}

	VkPipelineMultisampleStateCreateInfo& GraphicsPipelineBuilder::multisampleInfo(){
		return _multisampleInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo& GraphicsPipelineBuilder::inputAssemblyInfo(){
		return _inputAssemblyInfo;
	}

	VkPipelineColorBlendStateCreateInfo& GraphicsPipelineBuilder::colorBlendInfo(){
		return _colorBlendInfo;
	}

	VkPipelineDepthStencilStateCreateInfo& GraphicsPipelineBuilder::depthStencilInfo(){
		return _depthStencilInfo;
	}

	VkPipelineTessellationStateCreateInfo& GraphicsPipelineBuilder::tessellationInfo(){
		return _tessellationInfo;
	}

	VkPipelineColorBlendAttachmentState& GraphicsPipelineBuilder::attachmentState(uint32_t id){
		if (id > _colorAttachments.size()) throw std::runtime_error("out of bounds");
		return _colorAttachments[id];
	}

	void GraphicsPipelineBuilder::setVertexAttribtes(const std::vector<VkVertexInputAttributeDescription>& attributes){
		_vertexAttributes = attributes;
	}

	void GraphicsPipelineBuilder::setVertexBindings(const std::vector<VkVertexInputBindingDescription>& bindings){
		_vertexBindings = bindings;
	}

}