#pragma once

#include "Raindrop/Graphics/Backend/API.hpp"
namespace Raindrop::Graphics::Backend{
    class Surface{
        public:
            virtual ~Surface() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}