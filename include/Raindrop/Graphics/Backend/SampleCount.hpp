#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
namespace Raindrop::Graphics::Backend{
    enum class SampleCountBits{
        SAMPLE_1 = 1 << 0,
        SAMPLE_2 = 1 << 1,
        SAMPLE_4 = 1 << 2,
        SAMPLE_8 = 1 << 3,
        SAMPLE_16 = 1 << 4,
        SAMPLE_32 = 1 << 5,
        SAMPLE_64 = 1 << 6,
    };

    RAINDROP_FLAG(SampleCount, SampleCountBits);
}