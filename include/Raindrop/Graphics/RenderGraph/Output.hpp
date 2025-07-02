#pragma once

#include <string>

namespace Raindrop::Graphics::RenderGraph{
    class Output{
        public:
            virtual ~Output() = default;

            virtual const std::string& getName() const = 0;
    };
}