#pragma once

#include "Scene.hpp"

namespace Raindrop::Scene{
    class IBehavior{
        public:
            virtual ~IBehavior() = default;

            virtual void onCreate(Entity& entity);
            virtual void onDestroy(Entity& entity);

            virtual void initialize(Scene& scene);
            virtual void shutdown();

            virtual void preEvent();
            virtual void event();
            virtual void postEvent();

            virtual void preUpdate();
            virtual void update();
            virtual void postUpdate();

            virtual void preRender();
            virtual void render();
            virtual void postRender();
    };
}