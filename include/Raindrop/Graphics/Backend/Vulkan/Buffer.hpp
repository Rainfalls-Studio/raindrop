#pragma once

#include <vk_mem_alloc.h>
#include "../Buffer.hpp"
#include "Device.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
	class Buffer : public Backend::Buffer{
		public:
			Buffer(Device) noexcept;
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			Buffer(Buffer&& other);
			Buffer& operator=(Buffer&& other);

			friend void swap(Buffer& A, Buffer& B);

			void allocate();
			void free();

			Buffer& setSize(const std::size_t& size);
			Buffer& setAlignement(const std::size_t& alignement);
			Buffer& setUsage(const VkBufferUsageFlags& usage);
			Buffer& setCreateFlags(const VkBufferCreateFlags& flags);
			Buffer& setMemoryProperties(const VkMemoryPropertyFlags& properties);
			Buffer& setQueueFamilies(const std::vector<uint32_t>& families);
			Buffer& setSharingMode(const VkSharingMode& sharingMode);
			Buffer& setMemory(const std::shared_ptr<Memory>& memory);
			Buffer& setMemoryOffset(const std::size_t& offset);

			const VkBuffer& get() const noexcept;
			const std::shared_ptr<Memory>& getMemory() const noexcept;

			void map(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void unmap();

			bool isMapped() const noexcept;
			void* getMappedPointer() noexcept;

			void flush(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void invalidate(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);

		private:
			struct BuildInfo{
				std::size_t size;
				std::size_t alignement;
				VkBufferUsageFlags usage;
				VkBufferCreateFlags createFlags;
				VkMemoryPropertyFlags memoryPropertiess;
				std::vector<uint32_t> queueFamilies;
				VkSharingMode sharingMode;
				std::shared_ptr<Memory> memory;
				std::size_t memoryOffset;

				BuildInfo();
			};

			Context* _context;
			VkBuffer _buffer;
			std::shared_ptr<Memory> _memory;
			void* _mapped;
			std::size_t _size;

			std::unique_ptr<BuildInfo> _buildInfo;
			BuildInfo& getInfo();
	};
}