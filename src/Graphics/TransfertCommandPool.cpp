#include <Raindrop/Graphics/TransfertCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	TransfertCommandPool::TransfertCommandPool(GraphicsContext& context) : _context{context}{

		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = _context.transfertFamily;

		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if (vkCreateCommandPool(_context.device.get(), &info, _context.allocationCallbacks, &_primaryCommandPool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to create primary transfert command pool";
			throw std::runtime_error("Failed to create primary command pool");
			
		}

		info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		if (vkCreateCommandPool(_context.device.get(), &info, _context.allocationCallbacks, &_singleTimeCommandPool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to create single time transfert command pool";
			throw std::runtime_error("Failed to create single time command pool");
		}
	}

	TransfertCommandPool::~TransfertCommandPool(){
		if (_primaryCommandPool != VK_NULL_HANDLE) vkDestroyCommandPool(_context.device.get(), _primaryCommandPool, _context.allocationCallbacks);
		if (_singleTimeCommandPool != VK_NULL_HANDLE) vkDestroyCommandPool(_context.device.get(), _singleTimeCommandPool, _context.allocationCallbacks);
	}

	VkCommandPool TransfertCommandPool::primary() const{
		return _primaryCommandPool;
	}

	VkCommandPool TransfertCommandPool::singleTime() const{
		return _singleTimeCommandPool;
	}

	VkCommandBuffer TransfertCommandPool::beginSingleTime(){
		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = 1;
		info.commandPool = _singleTimeCommandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		
		VkCommandBuffer commandBuffer;
		if (vkAllocateCommandBuffers(_context.device.get(), &info, &commandBuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to allocate single time transfert command buffer";
			throw std::runtime_error("Failed to allocate single time transfert command buffer");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to begin single time transfert command buffer";
			throw std::runtime_error("Failed to begin single time transfert command buffer");
		}

		return commandBuffer;
	}

	void TransfertCommandPool::endSingleTime(VkCommandBuffer commandBuffer){
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to end single time transfert command buffer";
			throw std::runtime_error("Failed to end single time transfert command buffer");
		}

		VkSubmitInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pCommandBuffers = &commandBuffer;
		info.commandBufferCount = 1;
		info.signalSemaphoreCount = 0;
		info.waitSemaphoreCount = 0;

		if (vkQueueSubmit(_context.transfertQueue, 1, &info, VK_NULL_HANDLE) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.TransfertCommandPool") << "Failed to submit single time transfert command buffer";
			throw std::runtime_error("Failed to submit single time transfert command buffer");
		}

		vkQueueWaitIdle(_context.transfertQueue);

		vkFreeCommandBuffers(_context.device.get(), _singleTimeCommandPool, 1, &commandBuffer);
	}
}