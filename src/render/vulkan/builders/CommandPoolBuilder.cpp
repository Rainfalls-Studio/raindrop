#include "render/vulkan/builders/CommandPoolBuilder.hpp"

namespace rnd::render::vulkan{
	void CommandPoolBuilder::setFamily(QueueFamily family){
		this->family = family;
	}

	void CommandPoolBuilder::setFlag(VkCommandPoolCreateFlags flags){
		this->flags = flags;
	}

	void CommandPoolBuilder::setLogicalDevice(LogicalDevice* logicalDevice){
		this->device = logicalDevice;
	}
}