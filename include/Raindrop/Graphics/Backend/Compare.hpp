#pragma once

namespace Raindrop::Graphics::Backend{
    enum class CompareOp{
        NEVER = 0,
        LESS = 1,
        LESS_OR_EQUAL = 2,
        GREATER = 3,
        GREATER_OR_EQUAL = 4,
        EQUAL = 5,
        NOT_EQUAL = 6,
        ALWAYS = 7
    };
}