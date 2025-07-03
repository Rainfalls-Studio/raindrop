#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class DescriptorSet{
        public:
            struct Description{

            };

            virtual ~DescriptorSet() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}