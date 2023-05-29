#include <Raindrop/Graphics/GraphicsPipeline.hpp>

namespace Raindrop::Graphics{
	GraphicsPipeline::GraphicsPipeline(const VkGraphicsPipelineCreateInfo& info, std::vector<std::shared_ptr<Shader>> shaders, VkAllocationCallbacks* allocationCallbacks) : _allocationCallbacks{allocationCallbacks}, _shaders{shaders}{

	}

	GraphicsPipeline::~GraphicsPipeline(){

	}
}