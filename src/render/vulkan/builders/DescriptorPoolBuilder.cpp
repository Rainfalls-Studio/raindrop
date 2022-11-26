#include "render/vulkan/builders/DescriptorPoolBuilder.hpp"

namespace rnd::render::vulkan{
	void DescriptorPoolBuilder::setMaxSets(uint32_t count){
		maxSets = count;
	}

	void DescriptorPoolBuilder::setPoolFlags(VkDescriptorPoolCreateFlags flags){
		poolFlags = flags;
	}

	void DescriptorPoolBuilder::addPoolSize(VkDescriptorType type, uint32_t count){
		poolSizes.push_back({type, count});
	}

	void DescriptorPoolBuilder::setDevice(LogicalDevice* device){
		this->device = device;
	}

}