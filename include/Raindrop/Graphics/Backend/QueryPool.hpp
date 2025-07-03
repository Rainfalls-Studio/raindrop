#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class QueryPool{
        public:
            struct Description{

            };
            
            virtual ~QueryPool() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}