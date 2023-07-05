#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>

namespace Raindrop::Graphics::Builders{
	DescriptorPoolBuilder::DescriptorPoolBuilder(){

	}

	DescriptorPoolBuilder::~DescriptorPoolBuilder(){
		
	}

	void DescriptorPoolBuilder::setFlags(VkDescriptorPoolCreateFlags flags){
		_flags = flags;
	}

	void DescriptorPoolBuilder::setMaxSets(uint32_t max){
		_maxSets = max;
	}

	void DescriptorPoolBuilder::pushPoolSize(VkDescriptorPoolSize size){
		_poolSizes.push_back(size);
	}

	std::shared_ptr<DescriptorPool> DescriptorPoolBuilder::build(GraphicsContext& context){
		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.flags = _flags;
		info.maxSets = _maxSets;
		info.pPoolSizes = _poolSizes.data();
		info.poolSizeCount = static_cast<uint32_t>(_poolSizes.size());

		return std::make_shared<DescriptorPool>(context, info);
	}
}