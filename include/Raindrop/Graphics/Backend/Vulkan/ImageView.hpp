#pragma once

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class ImageView : public Object{
		public:
			static std::shared_ptr<ImageView> create(Raindrop::Context& context);

			ImageView() noexcept;
			~ImageView();

			ImageView(const ImageView&) = delete;
			ImageView& operator=(const ImageView&) = delete;

			ImageView(ImageView&& other);
			ImageView& operator=(ImageView&& other);

			friend void swap(ImageView& A, ImageView& B);

			ImageView& prepare(Context& context);
			void initialize();
			void release();

			ImageView& setSource(const std::shared_ptr<Image>& image);
			ImageView& setFlags(const VkImageViewCreateFlags& flags);
			ImageView& setType(const VkImageViewType& type);
			ImageView& setFormat(const VkFormat& format);
			ImageView& setComponentMapping(const VkComponentMapping& mapping);
			ImageView& setAspectMask(const VkImageAspectFlags& mask);
			ImageView& setBaseMipmapLevel(const std::uint32_t& level);
			ImageView& setMipmapLevelCount(const std::uint32_t& count);
			ImageView& setBaseArrayLayer(const std::uint32_t& layer);
			ImageView& setLayerCount(const std::uint32_t& count);

			const VkImageView& get() const noexcept;

		private:
			struct BuildInfo{
				std::shared_ptr<Image> source;
				VkImageViewCreateFlags flags;
				VkImageViewType viewType;
				VkFormat format;
				VkComponentMapping componentMapping;
				VkImageAspectFlags imageAspectMask;
				std::uint32_t baseMip;
				std::uint32_t mipCount;
				std::uint32_t baseLayer;
				std::uint32_t layerCount;

				BuildInfo();
			};

			Context* _context;
			VkImageView _imageView;
			std::shared_ptr<Image> _source;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
	};
}