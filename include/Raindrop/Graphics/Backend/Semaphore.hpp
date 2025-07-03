#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Semaphore{
        public:
            virtual ~Semaphore() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}