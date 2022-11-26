
#include "render/vulkan/Buffer.hpp"

namespace rnd::render::vulkan{

	static inline VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment){
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}

	void Buffer::alloc(VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryProperties){
		this->usageFlags = usageFlags;
		this->memoryPropertyFlags = memoryProperties;
		this->bufferSize = bufferSize;

		if (buffer != VK_NULL_HANDLE){
			destroy();
		}

		device->createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
	}

	void Buffer::init(LogicalDevice* device){
		this->device = device;
	}

	void Buffer::setInstanceProperties(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignement){
		this->instanceSize = instanceSize;
		alignmentSize = getAlignment(instanceSize, minOffsetAlignement);
		instanceCount = bufferSize / alignmentSize;
	}

	Buffer::~Buffer(){
		destroy();
	}

	void Buffer::destroy(){
		unmap();

		if (buffer){
			vkDestroyBuffer(device->getDevice(), buffer, nullptr);
			vkFreeMemory(device->getDevice(), memory, nullptr);
		}
	}


	VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset){
		return vkMapMemory(device->getDevice(), memory, offset, size, 0, &mapped);
	}

	void Buffer::unmap(){
		if (mapped == nullptr) return;
		vkUnmapMemory(device->getDevice(), memory);
		mapped = nullptr;
	}

	void Buffer::write(void *data, VkDeviceSize size, VkDeviceSize offset){

		if (size = VK_WHOLE_SIZE){
			memcpy(mapped, data, bufferSize);
		} else {
			char* memOffset = static_cast<char*>(mapped);
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset){
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(device->getDevice(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset){
		return VkDescriptorBufferInfo{buffer, offset, size};
	}

	VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset){
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(device->getDevice(), 1, &mappedRange);
	}

	void Buffer::writeToIndex(void* data, int index){
		write(data, instanceSize, index * alignmentSize);
	}

	VkResult Buffer::flushIndex(int index){
		return flush(alignmentSize, index * alignmentSize);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int index){
		return descriptorInfo(alignmentSize, index * alignmentSize);
	}

	VkResult Buffer::invalidateIndex(int index){
		return invalidate(alignmentSize, index * alignmentSize);
	}

	VkBuffer Buffer::getBuffer() const{
		return buffer;
	}

	void* Buffer::getMappedMemory() const{
		return mapped;
	}

	uint32_t Buffer::getInstanceCount() const{
		return instanceCount;
	}

	VkDeviceSize Buffer::getInstanceSize() const{
		return instanceSize;
	}

	VkDeviceSize Buffer::getAlignmentSize() const{
		return alignmentSize;
	}

	VkBufferUsageFlags Buffer::getUsageFlags() const{
		return usageFlags;
	}

	VkMemoryPropertyFlags Buffer::getMemoryPropertyFlags() const{
		return memoryPropertyFlags;
	}

	VkDeviceSize Buffer::getBufferSize() const{
		return bufferSize;
	}
}