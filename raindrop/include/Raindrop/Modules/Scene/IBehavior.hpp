#pragma once

#include "Scene.hpp"

namespace Raindrop::Scene{
    class IBehavior{
        public:
            virtual ~IBehavior() = default;

            virtual void onCreate(Entity& entity);
            virtual void onDestroy(Entity& entity);

            virtual void initialize(Engine& engine, Scene& scene);
            virtual void shutdown();
    };
}