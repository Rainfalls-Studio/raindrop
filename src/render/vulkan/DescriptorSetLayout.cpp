
#include "render/vulkan/DescriptorSetLayout.hpp"

#include <cassert>

namespace rnd::render::vulkan{
	DescriptorSetLayout::~DescriptorSetLayout(){
		if (device){
			vkDestroyDescriptorSetLayout(device->getDevice(), setLayout, nullptr);
		}
	}
	
	void DescriptorSetLayout::initialize(const DescriptorSetLayoutBuilder &builder){
		assert(builder.device != nullptr && "cannot initialize a descriptor set layout without a valid device");
		this->device = builder.device;
		bindings = builder.bindings;

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
		for (auto b : bindings){
			layoutBindings.push_back(b.second);
		}

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		createInfo.pBindings = layoutBindings.data();
		
		if (vkCreateDescriptorSetLayout(device->getDevice(), &createInfo, nullptr, &setLayout) != VK_SUCCESS){
			throw "failed to create descriptor set layout";
		}
		
	}
	
	VkDescriptorSetLayout DescriptorSetLayout::getDescriptorSetLayout(){
		return setLayout;
	}
}