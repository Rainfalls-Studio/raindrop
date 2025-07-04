#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class RenderPass{
        public:
            struct Description{

            };

            virtual ~RenderPass() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}