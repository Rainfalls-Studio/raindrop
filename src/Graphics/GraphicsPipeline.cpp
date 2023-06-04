#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Device.hpp>

namespace Raindrop::Graphics{
	GraphicsPipeline::GraphicsPipeline(const std::shared_ptr<Device>& device, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders, VkAllocationCallbacks* allocationCallbacks) : _allocationCallbacks{allocationCallbacks}, _shaders{shaders}, _device{device}{
		if (vkCreatePipelineLayout(_device->get(), &layoutInfo, allocationCallbacks, &_pipelineLayout) != VK_SUCCESS){
			throw std::runtime_error("Failed to create pipeline layout");
		}

		info.layout = _pipelineLayout;

		if (vkCreateGraphicsPipelines(_device->get(), VK_NULL_HANDLE, 1, &info, allocationCallbacks, &_pipeline) != VK_SUCCESS){
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		if (_pipeline) vkDestroyPipeline(_device->get(), _pipeline, _allocationCallbacks);
		if (_pipelineLayout) vkDestroyPipelineLayout(_device->get(), _pipelineLayout, _allocationCallbacks);
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
}