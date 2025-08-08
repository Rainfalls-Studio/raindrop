#pragma once

#include <functional>

namespace Raindrop::Store{
    template<typename T>
    struct ResourceBuilder {
        using Factory = std::function<T()>;
        Factory factory;
        ResourceBuilder() = default;
        explicit inline ResourceBuilder(Factory f) : factory(std::move(f)) {}
        inline T make() const { return factory(); }
    };
}