#pragma once

#include <string>
#include <cstdint>

namespace Raindrop::Graphics::Output{
    class IOutputTarget{
        public:
            virtual ~IOutputTarget() = default;

            struct Definition{
                std::string name;
                uint32_t bufferCount;
            };

            virtual Definition getDefinition() const = 0;
            virtual const char* name() const = 0;
    };
}