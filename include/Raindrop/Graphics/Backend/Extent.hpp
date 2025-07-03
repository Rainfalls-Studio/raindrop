#pragma once

#include <cstdint>

namespace Raindrop::Graphics::Backend{
    struct Extent2D{
        uint32_t width;
        uint32_t height;
    };

    struct Extent3D{
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    };
}