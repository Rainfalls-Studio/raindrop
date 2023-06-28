#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{

	VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}

	Buffer::Buffer(GraphicsContext& context) : _context{context}{}

	Buffer::~Buffer(){
		unmap();
		free();
	}

	void Buffer::allocate(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		free();
		
		_instanceSize = instanceSize;
		_instanceCount = instanceCount;
		_usageFlags = usageFlags;
		_memoryPropertyFlags = memoryPropertyFlags;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = instanceSize * instanceCount;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_context.device.get(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(_context.device.get(), _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = _context.device.findMemoryType(memRequirements.memoryTypeBits, memoryPropertyFlags);

		if (vkAllocateMemory(_context.device.get(), &allocInfo, nullptr, &_memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(_context.device.get(), _buffer, _memory, 0);
	}

	void Buffer::allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		_alignmentSize = getAlignment(_instanceSize, minOffsetAlignment);
		allocate(size, 1, usageFlags, memoryPropertyFlags, minOffsetAlignment);
	}

	void Buffer::free(){
		if (_buffer) vkDestroyBuffer(_context.device.get(), _buffer, nullptr);
		if (_memory) vkFreeMemory(_context.device.get(), _memory, nullptr);
	}

	VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset){
		return vkMapMemory(_context.device.get(), _memory, offset, size, 0, &_mapped);
	}

	void Buffer::unmap(){
		if (_mapped){
			vkUnmapMemory(_context.device.get(), _memory);
			_mapped = nullptr;
		}
	}

	void Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
		if (size == VK_WHOLE_SIZE) {
			memcpy(_mapped, data, _instanceSize * _instanceCount);
		} else {
			char *memOffset = (char *)_mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(_context.device.get(), 1, &mappedRange);
	}

	VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(_context.device.get(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{_buffer, offset, size};
	}

	void Buffer::writeToIndex(void *data, int index) {
		writeToBuffer(data, _instanceSize, index * _alignmentSize);
	}

	VkResult Buffer::flushIndex(int index) {
		return flush(_alignmentSize, index * _alignmentSize);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int index) {
		return descriptorInfo(_alignmentSize, index * _alignmentSize);
	}

	VkResult Buffer::invalidateIndex(int index) {
		return invalidate(_alignmentSize, index * _alignmentSize);
	}

	VkBuffer Buffer::get(){
		return _buffer;
	}

	VkDeviceMemory Buffer::memory(){
		return _memory;
	}

	void* Buffer::mapped(){
		return _mapped;
	}
}