#pragma once

#include <memory>

namespace Raindrop::Core::Memory{
    template<typename T>
    using Unique = std::unique_ptr<T>;
}