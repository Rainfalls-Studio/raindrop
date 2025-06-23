#pragma once

#include "Raindrop/Core/Utility/Flags.hpp"

namespace Raindrop::Window{
    enum class FlagBits{
        NONE = 0,
        RESIZED = 1 << 0,
        MOVED = 1 << 2
    };

    using Flags = Core::Utils::Flags<FlagBits>;
}