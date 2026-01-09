#pragma once

#include <glm/glm.hpp>
#include <string>
#include <Raindrop/Utils/Flags.hpp>

namespace Window{
    enum class WindowFlagBits {
        NONE = 0,
        FULLSCREEN = 1 << 0,
        BORDERLESS = 1 << 1,
        RESIZABLE = 1 << 2,
        UTILITY = 1 << 3,
        TRANSPARENT = 1 << 4,
        NOT_FOCUSABLE = 1 << 5,
        HIDDEN = 1 << 6
    };

    RAINDROP_FLAG(WindowFlags, WindowFlagBits)

    struct WindowConfig{
        glm::u32vec2 resolution;
        glm::ivec2 position;
        std::string title;
        WindowFlags flags;

        static WindowConfig Empty() noexcept;
    };
}