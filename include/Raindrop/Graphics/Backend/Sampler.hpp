#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
#include "Compare.hpp"
#include "Filter.hpp"

namespace Raindrop::Graphics::Backend{
    class Sampler{
        public:
            enum class FlagBits{
                NONE = 0,
                SUBSAMPLED = 1 << 0,
                SUBSAMPLED_COARSE_RECONSTRUCTION = 1 << 1,
                NON_SEAMLESS_CUBE_MAP = 1 << 2
            };

            RAINDROP_FLAG_CLASS(Flags, FlagBits)

            enum class MipmapMode{
                NEAREST = 0,
                LINEAR = 1
            };

            enum class AdressMode{
                REPEAT = 0,
                CLAMP_TO_EDGE = 1,
                CLAMP_TO_BORDER = 2,
                MIRRORED_REPEAT = 3
            };

            struct Description{
                Flags flags;
                Filter magFilter;
                Filter minFilter;
                MipmapMode mipmapMode;
                AdressMode adressModeU, adressModeV, adressModeW;
                float mipLodBias;
                bool anisotropyEnable;
                float maxAnisotropy;
                bool compareEnable;
                CompareOp compareOp;
                float minLod;
                float maxLod;
                bool normalizedCoordinates;
            };

            virtual ~Sampler() = default;
    };
}