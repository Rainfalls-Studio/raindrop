#pragma once

#include <memory>
namespace Raindrop::Core::Memory{
    template<typename T>
    using Weak = std::weak_ptr<T>;
}