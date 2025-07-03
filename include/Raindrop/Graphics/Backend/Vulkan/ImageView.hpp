#pragma once

#include "../ImageView.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class ImageView : public Backend::ImageView{
        public:
            ImageView(Context& context, const Description& description);
            virtual ~ImageView() override;

            VkImageView get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkImageView _imageView;
    };
}