#pragma once

#include <cstdint>
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class TimelineSemaphore{
        public:
            virtual ~TimelineSemaphore() = default;

            struct Description{
                uint32_t initialValue;
            };

            virtual void signal(uint32_t value) = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}