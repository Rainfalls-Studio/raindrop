#pragma once

namespace Raindrop::Graphics::Backend{
    enum class API{
        UNKNOWN = 0,
        VULKAN = 1,
        D3D12 = 2,
        Metal = 2,
        GL = 4
    };
}