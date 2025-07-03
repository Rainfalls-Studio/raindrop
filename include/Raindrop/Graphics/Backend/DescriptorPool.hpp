#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class DescriptorPool{
        public:
            struct Description{

            };

            virtual ~DescriptorPool() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}