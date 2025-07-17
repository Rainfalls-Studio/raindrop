#pragma once

#include <vector>
#include "ResourceDescription.hpp"

namespace Raindrop::Graphics::Common{
    class INode{
        public:
            virtual ~INode() = default;
            
            virtual std::vector<ResourceDescription> inputs() = 0;
            virtual std::vector<ResourceDescription> outputs() = 0;
    };
}