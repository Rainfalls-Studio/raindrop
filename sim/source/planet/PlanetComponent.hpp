#pragma once

#include <cstdint>
#include "PlanetServiceBehavior.hpp"

namespace Planet{
    struct PlanetComponent{
        float radius = 1.f;
        uint8_t maxLOD = 1;
        PlanetID id = INVALID_PLANET_ID;
        Raindrop::Scene::EntityHandle lodAnchor = Raindrop::Scene::INVALID_ENTITY_HANDLE;

        // component has been updated
        bool dirty = true;
    };
}