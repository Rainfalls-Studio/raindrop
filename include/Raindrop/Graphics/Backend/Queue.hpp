#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Queue{
        public:
            enum class CapabilitiesBits{
                NONE = 0,
                GRAPHICS = 1 << 0,
                COMPUTE = 1 << 1,
                TRANSFER = 1 << 2,
                PRESENT = 1 << 3,
                VIDEO_DECODE = 1 << 4,
                VIDEO_ENCODE = 1 << 5,
                __SIZE__
            };

            RAINDROP_FLAG_CLASS(Capabilities, CapabilitiesBits)

            virtual ~Queue() = default;
            
            virtual Capabilities getCapabilities() const = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}