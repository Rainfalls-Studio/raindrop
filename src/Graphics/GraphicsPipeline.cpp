#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	GraphicsPipeline::GraphicsPipeline(GraphicsContext& context, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders, std::string name) : _context{context}, _shaders{shaders}, _name{name}{
		if (vkCreatePipelineLayout(_context.device.get(), &layoutInfo, _context.allocationCallbacks, &_pipelineLayout) != VK_SUCCESS){
			throw std::runtime_error("Failed to create pipeline layout");
		}

		info.layout = _pipelineLayout;
		_context.gRegistry[name] = this;

		if (vkCreateGraphicsPipelines(_context.device.get(), VK_NULL_HANDLE, 1, &info, _context.allocationCallbacks, &_pipeline) != VK_SUCCESS){
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		if (_pipeline) vkDestroyPipeline(_context.device.get(), _pipeline, _context.allocationCallbacks);
		if (_pipelineLayout) vkDestroyPipelineLayout(_context.device.get(), _pipelineLayout, _context.allocationCallbacks);
	}

	void GraphicsPipeline::bind(VkCommandBuffer commandBuffer){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	}
	
	VkPipeline GraphicsPipeline::pipeline() const{
		return _pipeline;
	}

	VkPipelineLayout GraphicsPipeline::layout() const{
		return _pipelineLayout;
	}

	const std::string& GraphicsPipeline::name() const{
		return _name;
	}
}