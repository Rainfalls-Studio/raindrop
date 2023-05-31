#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Device.hpp>

namespace Raindrop::Graphics{
	GraphicsPipeline::GraphicsPipeline(const std::shared_ptr<Device>& device, const VkGraphicsPipelineCreateInfo& info, std::vector<std::shared_ptr<Shader>> shaders, VkAllocationCallbacks* allocationCallbacks) : _allocationCallbacks{allocationCallbacks}, _shaders{shaders}, _device{device}{
		if (vkCreateGraphicsPipelines(_device->get(), VK_NULL_HANDLE, 1, &info, allocationCallbacks, &_pipeline) != VK_SUCCESS){
			throw std::runtime_error("Failed to create vulkan pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		if (_pipeline) vkDestroyPipeline(_device->get(), _pipeline, _allocationCallbacks);
	}
}