#pragma once

#include <typeindex>

namespace Layers{
    struct Transmission{
        virtual ~Transmission() = default;
        
        std::type_index type() const noexcept{
            return typeid(*this);
        }
    };
}