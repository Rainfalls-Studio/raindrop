#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>

namespace Raindrop::Graphics::Builders{
	DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(){

	}

	DescriptorSetLayoutBuilder::~DescriptorSetLayoutBuilder(){

	}

	std::shared_ptr<DescriptorSetLayout> DescriptorSetLayoutBuilder::build(GraphicsContext& context){
		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.flags = _flags;
		info.bindingCount = static_cast<uint32_t>(_bindings.size());
		info.pBindings = _bindings.data();

		return std::make_shared<DescriptorSetLayout>(context, info);
	}

	void DescriptorSetLayoutBuilder::setFlags(VkDescriptorSetLayoutCreateFlags flags){
		_flags = flags;
	}

	void DescriptorSetLayoutBuilder::pushBinding(VkDescriptorSetLayoutBinding& binding){
		_bindings.push_back(binding);
	}
}
