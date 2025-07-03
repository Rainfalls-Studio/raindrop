#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class PipelineCache{
        public:
            struct Description{

            };
            
            virtual ~PipelineCache() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}