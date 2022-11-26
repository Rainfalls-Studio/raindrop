
#include "render/vulkan/DescriptorPool.hpp"

#include <cassert>

namespace rnd::render::vulkan{
	DescriptorPool::~DescriptorPool(){
		if (pool){
			vkDestroyDescriptorPool(device->getDevice(), pool, nullptr);
		}
	}

	void DescriptorPool::init(DescriptorPoolBuilder &builder){
		device = builder.device;
		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount = static_cast<uint32_t>(builder.poolSizes.size());
		createInfo.pPoolSizes = builder.poolSizes.data();
		createInfo.flags = builder.poolFlags;
		createInfo.maxSets = builder.maxSets;

		if (vkCreateDescriptorPool(device->getDevice(), &createInfo, nullptr, &pool) != VK_SUCCESS){
			throw "failed to create descriptor pool";
		}
	}

	bool DescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor){
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;

		return vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptor) == VK_SUCCESS;
	}

	void DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors){
		vkFreeDescriptorSets(device->getDevice(), pool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
	}

	void DescriptorPool::resetPool(){
		vkResetDescriptorPool(device->getDevice(), pool, 0);
	}

	VkDescriptorPool DescriptorPool::getPool(){
		return pool;
	}
}