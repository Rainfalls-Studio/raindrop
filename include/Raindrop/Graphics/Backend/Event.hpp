#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Event{
        public:
            struct Description{

            };
            
            virtual ~Event() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}