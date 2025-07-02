#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
#include "Format.hpp"
#include "Extent.hpp"
#include "Queue.hpp"
#include "SampleCount.hpp"
#include <list>
#include <memory>

namespace Raindrop::Graphics::Backend{
    class Image{
        public:
            enum class FlagBits{
                NONE = 0,
                MUTABLE_FORMAT = 1 << 0,
                COMPATIBLE_CUBE = 1 << 1,
            };

            RAINDROP_FLAG_CLASS(Flags, FlagBits);

            enum class UsageBits{
                NONE = 0,
                TRANSFER_SRC = 1 << 0,
                TRANSFER_DST = 1 << 1,
                SAMPLED = 1 << 2,
                STORAGE = 1 << 3,
                COLOR_ATTACHMENT = 1 << 4,
                DEPTH_STENCIL_ATTACHMENT = 1 << 5,
                TRANSIENT_ATTACHMENT = 1 << 6,
                INPUT_ATTACHMENT = 1 << 7,
                FRAGMENT_DENSITY_MAP = 1 << 8,
                FRAGMENT_SHADING_RATE = 1 << 9,
                VIDEO_DECODE_SRC = 1 << 10,
                VIDEO_DECODE_DST = 1 << 11,
                VIDEO_DECODE_DPB = 1 << 12,
                VIDEO_ENCORE_SRC = 1 << 13,
                VIDEO_ENCORE_DST = 1 << 14,
                VIDEO_ENCORE_DPB = 1 << 15,
                ATTACHMENT_FEEDBACK_LOOP = 1 << 16,
            };

            RAINDROP_FLAG_CLASS(Usage, UsageBits)

            enum class Type{
                D1 = 1,
                D2 = 2,
                D3 = 3,
            };

            enum class Tiling{
                OPTIMAL,
                LINEAR
            };

            enum class Layout{
                UNDEFINED,
                GENERAL,
                COLOR_ATTACHMENT_OPTIMAL,
                DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                DEPTH_STENCIL_READ_ONLY_OPTIMAL,
                TRANSFER_SRC_OPTIMAL,
                TRANSFER_DST_OPTIMAL,
                PREINITIALIZED,
                DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
                DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
                DEPTH_ATTACHMENT_OPTIMAL,
                DEPTH_READ_ONLY_OPTIMAL,
                STENCIL_ATTACHMENT_OPTIMAL,
                STENCIL_READ_ONLY_OPTIMAL,
                READ_ONLY_OPTIMAL,
                ATTACHMENT_OPTIMAL,
                RENDERING_LOCAL_READ,
                PRESENT_SRC,
                SHARED_PRESENT_SRC,
                VIDEO_DECODE_DST,
                VIDEO_DECODE_SRC,
                VIDEO_DECODE_DPB,
                VIDEO_ENCODE_DST,
                VIDEO_ENCODE_SRC,
                VIDEO_ENCODE_DPB,
            };

            enum class AspectBits{
                NONE = 0,
                COLOR = 1 << 0,
                DEPTH = 1 << 1,
                STENCIL = 1 << 2,
                METADATA = 1 << 3,
                PLANE_0 = 1 << 4,
                PLANE_1 = 1 << 5,
                PLANE_2 = 1 << 6,
                MEMORY_PLANE_0 = 1 << 7,
                MEMORY_PLANE_1 = 1 << 8,
                MEMORY_PLANE_2 = 1 << 9,
                MEMORY_PLANE_3 = 1 << 10
            };

            RAINDROP_FLAG_CLASS(Aspect, AspectBits);

            struct SubresourceRange{
                Aspect aspect;
                uint32_t baseMipLevel;
                uint32_t levelCount;
                uint32_t baseArrayLevel;
                uint32_t arrayCount;
            };

            struct Description{
                Flags flags;
                Type type;
                Format format;
                Extent3D extent;
                uint32_t mipLevels;
                uint32_t arrayLayers;
                SampleCount samples;
                Tiling tiling;
                Usage usage;
                Layout layout;
                std::list<std::shared_ptr<Queue>> queues;
            };
    };
}