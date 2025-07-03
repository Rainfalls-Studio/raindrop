#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Framebuffer{
        public:
            struct Description{

            };

            virtual ~Framebuffer() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}