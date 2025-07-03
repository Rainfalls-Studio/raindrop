#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class CommandList{
        public:
            virtual ~CommandList() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}