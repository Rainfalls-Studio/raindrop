#include <Raindrop/Graphics/GraphicsCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	GraphicsCommandPool::GraphicsCommandPool(GraphicsContext& context) : _context{context}{

		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = _context.graphicsFamily;

		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if (vkCreateCommandPool(_context.device.get(), &info, _context.allocationCallbacks, &_primaryCommandPool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to create primary graphics command pool";
			throw std::runtime_error("Failed to create primary command pool");
			
		}

		info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		if (vkCreateCommandPool(_context.device.get(), &info, _context.allocationCallbacks, &_singleTimeCommandPool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to create single time graphics command pool";
			throw std::runtime_error("Failed to create single time command pool");
		}
	}

	GraphicsCommandPool::~GraphicsCommandPool(){
		if (_primaryCommandPool != VK_NULL_HANDLE) vkDestroyCommandPool(_context.device.get(), _primaryCommandPool, _context.allocationCallbacks);
		if (_singleTimeCommandPool != VK_NULL_HANDLE) vkDestroyCommandPool(_context.device.get(), _singleTimeCommandPool, _context.allocationCallbacks);
	}
	
	VkCommandPool GraphicsCommandPool::primary() const{
		return _primaryCommandPool;
	}

	VkCommandPool GraphicsCommandPool::singleTime() const{
		return _singleTimeCommandPool;
	}

	VkCommandBuffer GraphicsCommandPool::beginSingleTime(){
		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = 1;
		info.commandPool = _singleTimeCommandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		
		VkCommandBuffer commandBuffer;
		if (vkAllocateCommandBuffers(_context.device.get(), &info, &commandBuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to allocate single time graphics command buffer";
			throw std::runtime_error("Failed to allocate single time graphics command buffer");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to begin single time graphics command buffer";
			throw std::runtime_error("Failed to begin single time graphics command buffer");
		}

		return commandBuffer;
	}

	void GraphicsCommandPool::endSingleTime(VkCommandBuffer commandBuffer){
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to end single time graphics command buffer";
			throw std::runtime_error("Failed to end single time graphics command buffer");
		}

		VkSubmitInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pCommandBuffers = &commandBuffer;
		info.commandBufferCount = 1;
		info.signalSemaphoreCount = 0;
		info.waitSemaphoreCount = 0;

		if (vkQueueSubmit(_context.graphicsQueue, 1, &info, VK_NULL_HANDLE) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GraphicsCommandPool") << "Failed to submit single time graphics command buffer";
			throw std::runtime_error("Failed to submit single time graphics command buffer");
		}

		vkFreeCommandBuffers(_context.device.get(), _singleTimeCommandPool, 1, &commandBuffer);
	}
}