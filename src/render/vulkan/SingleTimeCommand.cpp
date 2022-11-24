
#include "render/vulkan/SingleTimeCommand.hpp"

#include <cassert>

namespace rnd::render::vulkan{
	

	VkCommandBuffer SingleTimeCommand::getCommandBuffer(){
		return commandBuffer;
	}

	void SingleTimeCommand::begin(){
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandPool = commandPool->getCommandPool();
		info.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device->getDevice(), &info, &commandBuffer) != VK_SUCCESS){
			throw "failed to allocate command buffer";
		}

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	SingleTimeCommand::SingleTimeCommand(LogicalDevice* device, QueueFamily family, uint32_t queueIndex){
		assert(device != nullptr && "cannot create a single time command buffer without a valid device");

		this->device = device;
		this->queue = device->getQueue(family, queueIndex);
		this->commandPool = device->getCommandPool(family);
		begin();
	}

	void SingleTimeCommand::end(){
		if (commandBuffer){
			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(queue);

			vkFreeCommandBuffers(device->getDevice(), commandPool->getCommandPool(), 1, &commandBuffer);
			commandBuffer = VK_NULL_HANDLE;
		}
	}

	SingleTimeCommand::~SingleTimeCommand(){
		end();
	}
}