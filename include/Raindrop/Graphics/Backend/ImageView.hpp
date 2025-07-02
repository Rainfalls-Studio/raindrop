#pragma once

#include "Image.hpp"
#include "Raindrop/Graphics/Backend/Component.hpp"
#include "Raindrop/Graphics/Backend/Format.hpp"
#include <memory>

namespace Raindrop::Graphics::Backend{
    class ImageView{
        public:
            enum class Type{
                D1,
                D2,
                D3,
                CUBE,
                D1_ARRAY,
                D2_ARRAY,
                CUBE_ARRAY
            };

            struct Description{
                std::shared_ptr<Image> image;
                Format format;
                Type type;
                ComponentMapping components;
                Image::SubresourceRange range;
            };

            virtual ~ImageView() = default;
    };
}