#pragma once

#include <cstdint>
#include "Raindrop/Core/Utils/Flags.hpp"
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Fence{
        public:
            enum class Status{
                SIGNALED,
                NOT_SIGNALED
            };

            enum class FlagBits{
                NONE = 0,
                SIGNALED = 1 << 0,
                __SIZE__
            };

            RAINDROP_FLAG_CLASS(Flags, FlagBits)

            struct Description{
                Flags flags;
            };

            virtual ~Fence() = default;
            
            virtual void wait(uint32_t timeout = UINT32_MAX) = 0;
            virtual void reset() = 0;
            virtual Status getStatus() const = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}