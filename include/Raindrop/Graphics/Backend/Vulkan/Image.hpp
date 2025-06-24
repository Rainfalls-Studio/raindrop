#pragma once

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class Image : public Object{
		public:
			static std::shared_ptr<Image> create(Raindrop::Context& context);

			Image() noexcept;
			~Image();

			Image(const Image&) = delete;
			Image& operator=(const Image&) = delete;

			Image(Image&& other);
			Image& operator=(Image&& other);

			friend void swap(Image& A, Image& B);

			Image& prepare(Context& context);
			void initialize();
			void release();

			Image& setFlags(const VkImageCreateFlags& flags);
			Image& setType(const VkImageType& type);
			Image& setFormat(const VkFormat& format);
			Image& setExtent(const VkExtent3D& extent);
			Image& setWidth(const std::uint32_t& width);
			Image& setHeight(const std::uint32_t& height);
			Image& setDepth(const std::uint32_t& depth);
			Image& setExtent(const std::uint32_t& width, const std::uint32_t &height = 1, const std::uint32_t& depth = 1);
			Image& setMipmapLevels(const std::uint32_t& levels);
			Image& setArrayLayers(const std::uint32_t& layers);
			Image& setSampleCount(const VkSampleCountFlagBits& samples);
			Image& setTiling(const VkImageTiling& tiling);
			Image& setUsage(const VkImageUsageFlags& usage);
			Image& setSharingMode(const VkSharingMode& mdoe);
			Image& setQueueFamilies(const std::vector<uint32_t> families);
			Image& setInitialLayout(const VkImageLayout& layout);
			Image& setMemoryProperties(const VkMemoryPropertyFlags& properties);
			Image& setMemory(const std::shared_ptr<Memory>& memory);
			Image& setMemoryOffset(const std::size_t& offset);

			const VkImage& get() const noexcept;

			VkMemoryRequirements getMemoryRequirements() const;
			const std::shared_ptr<Memory> getMemory() const noexcept;

		private:
			struct BuildInfo{
				VkImageCreateInfo info;
				std::vector<std::uint32_t> familyIndices;
				std::shared_ptr<Memory> memory;
				std::size_t memoryOffset;
				VkMemoryPropertyFlags memoryProperties;

				BuildInfo();
			};

			Context* _context;
			VkImage _image;
			std::shared_ptr<Memory> _memory;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
			void checkInitialize() const;
	};
}