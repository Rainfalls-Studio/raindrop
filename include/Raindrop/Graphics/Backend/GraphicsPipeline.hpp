#pragma once

#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class GraphicsPipeline{
        public:
            struct Description{

            };

            virtual ~GraphicsPipeline() = default;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}