#pragma once

#include <stdexcept>
#include <vulkan/vulkan.h>
#include "../Buffer.hpp"
#include "Context.hpp"
#include "translation.hpp"
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

	class Buffer : public Backend::Buffer{
		public:
			Buffer(Context& context, const Description& buffer);
			virtual ~Buffer() override;

            virtual void* map(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void unmap() override;

            virtual void flush(Size size = SIZE_WHOLE, Size offset = 0) override;
            virtual void invalidate(Size size = SIZE_WHOLE, Size offset = 0) override;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

		private:
			Context& _context;
			VkBuffer _buffer = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
	};
}