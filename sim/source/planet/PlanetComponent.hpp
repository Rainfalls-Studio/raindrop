#pragma once

#include <cstdint>
#include "PlanetServiceBehavior.hpp"

namespace Planet{
    struct PlanetComponent{
        float radius;
        uint8_t maxLOD;
        PlanetID id;
        Raindrop::Scene::EntityHandle lodAnchor;

        // component has been updated
        bool dirty;
    };
}