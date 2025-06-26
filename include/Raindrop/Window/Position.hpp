#pragma once

#include "glm/glm.hpp"
#include <cmath>
#include <limits>

namespace Raindrop::Window{
    using PositionComponent = int;
    using Position = glm::vec<2, PositionComponent>;

    static constexpr PositionComponent POSITION_UNDEFINED = std::numeric_limits<PositionComponent>::max();
    static constexpr PositionComponent POSITION_CENTRED = std::numeric_limits<PositionComponent>::min();
}