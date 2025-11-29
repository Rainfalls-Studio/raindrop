#pragma once

#include <typeindex>

namespace Raindrop::Layers{
    struct Transmission{
        virtual ~Transmission() = default;
        
        std::type_index type() const noexcept{
            return typeid(*this);
        }
    };
}