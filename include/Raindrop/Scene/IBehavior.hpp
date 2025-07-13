#pragma once

namespace Raindrop::Scene{
    class IBehavior{
        public:
            virtual ~IBehavior() = default;

            virtual void initialize();
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