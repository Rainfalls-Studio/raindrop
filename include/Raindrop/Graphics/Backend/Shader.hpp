#pragma once

#include <string>
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Shader{
        public:
            struct Description{
                std::string code;
            };

            virtual ~Shader() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}