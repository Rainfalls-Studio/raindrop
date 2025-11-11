#pragma once

#include "Raindrop/Modules/Scene/Scene.hpp"

namespace Raindrop::Components{
    class Hierarchy{
        public:
            // Store entity handles and not entity wrappers to save space

            Scene::EntityHandle parent = Scene::INVALID_ENTITY_HANDLE;
            std::vector<Scene::EntityHandle> children;

            inline bool isChildOf(Scene::Scene& scene, Scene::EntityHandle entity) const{
                Scene::EntityHandle current = parent;

                while (current != Scene::INVALID_ENTITY_HANDLE){
                    if (current == entity) return true;

                    auto hierarchy = scene.getComponent<Hierarchy>(current);
                    current = hierarchy.parent;
                }
                return false;
            }
    };
}