#pragma once

#include "Raindrop/Scene/Scene.hpp"

namespace Raindrop::Builtin::Components{
    class Hierarchy{
        public:
            // Store entity handles and not entity wrappers to save space

            Scene::EntityHandle parent = Scene::INVALID_ENTITY_HANDLE;
            std::vector<Scene::EntityHandle> children;
    };
}