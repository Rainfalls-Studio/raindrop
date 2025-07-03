#pragma once

#include <cstdint>
#include <list>
#include "Raindrop/Core/Utils/Flags.hpp"
#include "Queue.hpp"
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Buffer{
        public:
            enum class UsageBits{
                NONE = 0,
                TRANSFER_SRC = 1 << 0,
                TRANSFER_DST = 1 << 1,
                UNIFORM_TEXEL_BUFFER = 1 << 2,
                STORAGE_TEXEL_BUFFER = 1 << 3,
                UNIFORM_BUFFER = 1 << 4,
                STORAGE_BUFFER = 1 << 5,
                INDEX_BUFFER = 1 << 6,
                VERTEX_BUFFER = 1 << 7,
                INDIRECT_BUFFER = 1 << 8,
                SHADER_DEVICE_ADDRESS = 1 << 9,
                VIDEO_DECODE_SRC = 1 << 10,
                VIDEO_DECODE_DST = 1 << 11,
                ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY = 1 << 12,
                ACCELERATION_STRUCTURE_STORAGE = 1 << 13,
                SHADER_BINDING_TABLE = 1 << 14,
                VIDEO_ENCODE_DST = 1 << 15,
                VIDEO_ENCODE_SRC = 1 << 16,
                SAMPLER_DESCRIPTOR_BUFFER = 1 << 17,
                RESOURCE_DESCRIPTOR_BUFFER = 1 << 18,
                PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER = 1 << 19,
                MICROMAP_BUILD_INPUT_READ_ONLY = 1 << 20,
                MICROMAP_STORAGE = 1 << 21,
                __SIZE__
            };

            RAINDROP_FLAG_CLASS(Usage, Buffer::UsageBits)
            
            using Size = uint32_t;
            static constexpr Size SIZE_WHOLE = ~static_cast<Size>(0);

            struct Description{
                Size size;
                Usage usage;
                std::list<Queue> queues;
            };
            
            virtual ~Buffer() = default;
            
            virtual void* map(Size size = SIZE_WHOLE, Size offset = 0) = 0;
            virtual void unmap() = 0;

            virtual void flush(Size size = SIZE_WHOLE, Size offset = 0) = 0;
            virtual void invalidate(Size size = SIZE_WHOLE, Size offset = 0) = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}