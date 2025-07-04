#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class AccelerationStructure{
        public:
            struct Description{
                
            };
            
            virtual ~AccelerationStructure() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}