#pragma once

#include <typeindex>

namespace Raindrop::Layers{
    struct TransmissionBase{
        virtual ~TransmissionBase() = default;
        virtual std::type_index type() const noexcept = 0;
    };

    template<typename T>
    struct Transmission{
        virtual ~Transmission() = default;

        constexpr inline virtual std::type_index type() const noexcept final{
            return typeid(T);
        }
    };
}