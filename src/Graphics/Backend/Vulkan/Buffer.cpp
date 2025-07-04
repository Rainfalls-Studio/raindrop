#include "Raindrop/Graphics/Backend/Vulkan/Buffer.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
	VkBufferUsageFlagBits toVulkan(Backend::Buffer::UsageBits&& bit){
		using enum Backend::Buffer::UsageBits;
		switch (bit){
			case NONE: return VkBufferUsageFlagBits(0);
			case TRANSFER_SRC: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			case TRANSFER_DST: return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			case UNIFORM_TEXEL_BUFFER: return VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
			case STORAGE_TEXEL_BUFFER: return VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			case UNIFORM_BUFFER: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			case STORAGE_BUFFER: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			case INDEX_BUFFER: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			case VERTEX_BUFFER: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			case INDIRECT_BUFFER: return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
			case SHADER_DEVICE_ADDRESS: return VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
			case VIDEO_DECODE_SRC: return VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
			case VIDEO_DECODE_DST: return VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR;
			case ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY: return VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
			case ACCELERATION_STRUCTURE_STORAGE: return VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
			case SHADER_BINDING_TABLE: return VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR;
			case VIDEO_ENCODE_DST: return VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
			case VIDEO_ENCODE_SRC: return VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
			case SAMPLER_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT;
			case RESOURCE_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
			case PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT;
			case MICROMAP_BUILD_INPUT_READ_ONLY: return VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT;
			case MICROMAP_STORAGE: return VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT;
			default: break;
		};
		throw std::runtime_error("Undefined buffer usage");
	}

	RAINDROP_TO_VK_FLAG(VkBufferUsageFlags, VkBufferUsageFlagBits, Backend::Buffer::Usage)

	Buffer::Buffer(Context& context, const Description& buffer) :
        _context{context}
    {
        
    }

    Buffer::~Buffer(){
        if (_buffer){
            vkDestroyBuffer(_context.device, _buffer, nullptr);
            _buffer = VK_NULL_HANDLE;
        }
    }


    void* Buffer::map(Size size, Size offset){
        // vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void **ppData)
    }

    void Buffer::unmap(){

    }

    void Buffer::flush(Size size, Size offset){

    }

    void Buffer::invalidate(Size size, Size offset){

    }

    void* Buffer::getHandle() const noexcept{
        return static_cast<void*>(_buffer);
    }

    API Buffer::getAPI() const noexcept{
        return API::VULKAN;
    }
}