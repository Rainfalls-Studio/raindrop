#ifndef __RAINDROP_GRAPHICS_BUILDERS_GRAPHICS_PIPELINE_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_GRAPHICS_PIPELINE_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

//TODO: Add 'loadFromXML' option - https://trello.com/c/ZkdVzN1X/3-add-loadfromxml-option
//TODO: custom vertex input - https://trello.com/c/ZpnG1ebY/4-custom-vertex-input
namespace Raindrop::Graphics::Builders{
	class GraphicsPipelineBuilder{
		public:
			GraphicsPipelineBuilder();
			~GraphicsPipelineBuilder();

			std::shared_ptr<GraphicsPipeline> build(GraphicsContext& context);

			void addShader(const std::shared_ptr<Shader>& shader);
			void addDescriptorSetLayout(const VkDescriptorSetLayout &layout);
			void setRenderPass(VkRenderPass renderPass);
			void setName(const std::string& name);
			void setAttachmentCount(uint32_t count);
			void setVertexAttribtes(const std::vector<VkVertexInputAttributeDescription>& attributes);
			void setVertexBindings(const std::vector<VkVertexInputBindingDescription>& bindings);
			
			VkPipelineViewportStateCreateInfo& viewportInfo();
			VkPipelineRasterizationStateCreateInfo& rasterizationInfo();
			VkPipelineMultisampleStateCreateInfo& multisampleInfo();
			VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo();
			VkPipelineColorBlendStateCreateInfo& colorBlendInfo();
			VkPipelineDepthStencilStateCreateInfo& depthStencilInfo();
			VkPipelineTessellationStateCreateInfo& tessellationInfo();
			VkPipelineColorBlendAttachmentState& attachmentState(uint32_t id);

		private:
			VkRenderPass _renderPass = VK_NULL_HANDLE;
			uint32_t _subpass = 0;

			std::vector<std::shared_ptr<Shader>> _shaders;
			std::vector<VkPipelineColorBlendAttachmentState> _colorAttachments;
			std::vector<VkDescriptorSetLayout> _setLayouts;
			std::vector<VkVertexInputAttributeDescription> _vertexAttributes;
			std::vector<VkVertexInputBindingDescription> _vertexBindings;
			std::string _name;

			VkPipelineLayout createPipelineLayout(VkAllocationCallbacks* callbacks);
			void destroyPipelineLayout(VkPipelineLayout layout, VkAllocationCallbacks* callbacks);

			VkPipelineViewportStateCreateInfo _viewportInfo{};
			VkPipelineRasterizationStateCreateInfo _rasterizationInfo{};
			VkPipelineMultisampleStateCreateInfo _multisampleInfo{};
			VkPipelineInputAssemblyStateCreateInfo _inputAssemblyInfo{};
			VkPipelineColorBlendStateCreateInfo _colorBlendInfo{};
			VkPipelineDepthStencilStateCreateInfo _depthStencilInfo{};
			VkPipelineTessellationStateCreateInfo _tessellationInfo{};
	};
}

#endif