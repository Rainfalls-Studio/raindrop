#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class DescriptorSetLayout{
        public:
            struct Description{

            };

            virtual ~DescriptorSetLayout() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}