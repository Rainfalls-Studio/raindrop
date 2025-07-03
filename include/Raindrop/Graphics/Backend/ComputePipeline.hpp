#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class ComputePipeline{
        public:
            virtual ~ComputePipeline() = default;

            struct Description{

            };

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}