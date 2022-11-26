#include "render/vulkan/builders/DescriptorSetBuilder.hpp"

namespace rnd::render::vulkan{
	void DescriptorSetBuilder::setDescriptors(const std::vector<Descriptor> &descriptors){
		this->descriptors = descriptors;
	}

	void DescriptorSetBuilder::setDescriptorSetCount(uint32_t count){
		descriptorSetCount = count;
	}

	void DescriptorSetBuilder::setLogicalDevice(LogicalDevice* device){
		this->device = device;
	}
}