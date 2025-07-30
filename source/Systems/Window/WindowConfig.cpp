#include "Raindrop/Systems/Window/WindowConfig.hpp"

namespace Raindrop::Window{
    WindowConfig WindowConfig::Empty() noexcept{
        return WindowConfig{
            .resolution = {1, 1},
            .position = {0, 0},
            .title = "",
            .flags = {
                WindowFlags::HIDDEN,
                WindowFlags::NOT_FOCUSABLE,
                WindowFlags::UTILITY
            }
        };
    }
}