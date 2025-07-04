#pragma once

#include "Scene.hpp"

namespace Raindrop::Scene{
    class Entity{
        public:
            Entity(Scene* scene, EntityID id);
            ~Entity();

            

        private:
            Scene* _scene;
            EntityID _id;
    };
}