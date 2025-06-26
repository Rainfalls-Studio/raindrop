#pragma once

#include "Raindrop/Core/Utility/Flags.hpp"

namespace Raindrop::Window{
    enum FlagBits{
        NONE = 0,
        FULLSCREEN = 1 << 0,
        BORDERLESS = 1 << 1,
        RESIZABLE = 1 << 2,
        MINIMIZED = 1 << 3,
        MAXIMIZED = 1 << 4,
        MODAL = 1 << 5,
        ALWAYS_ON_TOP = 1 << 6,
        UTILITY = 1 << 7,
        TOOLTIP = 1 << 8,
        POPUP_MENU = 1 << 9,
        TRANSPARENT = 1 << 10,
        HIDDEN = 1 << 11,
        NOT_FOCUSABLE = 1 << 12,
        MOUSE_CAPTURE = 1 << 13
    };

    RAINDROP_FLAG(Flags, FlagBits);
}