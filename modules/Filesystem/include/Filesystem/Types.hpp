#pragma once

#include "Raindrop/Utils/Flags.hpp"
#include <cstddef>
#include <string>
#include <string_view>

namespace Filesystem{
    using Offset = size_t;
    using Size = size_t;

    using String = std::string;
    using StringView = std::string_view;

    enum class SeekOrigin{
        BEGIN,
        CURRENT,
        END
    };

    enum class OpenFlagBits{
        NONE = 0,
        READ = 1 << 0,
        WRITE = 1 << 1,
        BINARY = 1 << 2,
        APPEND = 1 << 3,
        TRUCATE = 1 << 4,
        END = 1 << 5,
        EXCLUSIVE = 1 << 6
    };

    RAINDROP_FLAG(OpenFlags, OpenFlagBits)
}