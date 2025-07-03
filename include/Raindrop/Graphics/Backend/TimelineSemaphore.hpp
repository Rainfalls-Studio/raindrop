#pragma once

#include <cstdint>

namespace Raindrop::Graphics::Backend{
    class TimelineSemaphore{
        public:
            virtual ~TimelineSemaphore() = default;

            struct Description{
                uint32_t initialValue;
            };

            virtual void signal(uint32_t value) = 0;
    };
}