#pragma once

namespace Raindrop::Graphics::Backend{
    enum class ComponentSwizzle{
        IDENTITY,
        ZERO,
        ONE,
        R,
        G,
        B,
        A
    };

    class ComponentMapping{
        ComponentSwizzle r, g, b, a;
    };
}