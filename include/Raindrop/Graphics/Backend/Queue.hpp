#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
namespace Raindrop::Graphics::Backend{
    class Queue{
        public:
            enum class CapabilitiesBits{
                NONE = 0,
                GRAPHICS = 1 << 0,
                COMPUTE = 1 << 1,
                TRANSFER = 1 << 2,
                VIDEO_DECODE = 1 << 3,
                VIDEO_ENCORE = 1 << 4,
            };

            RAINDROP_FLAG_CLASS(Capabilities, CapabilitiesBits)

            virtual ~Queue() = default;
            
            virtual Capabilities getCapabilities() const = 0;
    };
}