#pragma once

#include <typeindex>

namespace Raindrop::Layers{
    struct TransmissionBase{
        virtual ~TransmissionBase() = default;
        virtual std::type_index type() const noexcept = 0;
    };

    template<typename T>
    struct Transmission : public TransmissionBase{
        virtual ~Transmission() = default;

        virtual std::type_index type() const noexcept final override;
    };

    template<typename T>
    std::type_index Transmission<T>::type() const noexcept {
        return typeid(T);
    }
}