#include "render/vulkan/CommandPool.hpp"
#include "render/vulkan/utils.hpp"

#include <stdexcept>
#include <cassert>

namespace rnd::render::vulkan{
	CommandPool::~CommandPool(){
		if (commandPool) vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
	}

	void CommandPool::init(CommandPoolBuilder &builder){
		assert(builder.device != nullptr && "cannot create a command pool without a valid logical device");
		assert(builder.family != FAMILY_NONE && "cannot create a command pool without a defined family");

		device = builder.device;
		flags = builder.flags;
		family = builder.family;
		
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = device->getPhysicalDevice()->getFamily(family).family;
		createInfo.flags = flags;

		if (vkCreateCommandPool(device->getDevice(), &createInfo, nullptr, &commandPool)){
			throw "failed to create a command pool";
		}
	}

	VkCommandPool CommandPool::getCommandPool(){
		return commandPool;
	}

	QueueFamily CommandPool::getFamily(){
		return family;
	}
}