#pragma once

#include <memory>

namespace Raindrop::Core::Memory{
    template<typename T>
    using Ref = std::shared_ptr<T>;
}