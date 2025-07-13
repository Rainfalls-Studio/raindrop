#pragma once

#include <typeindex>
#include <vector>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::System{
    class ISystem{
        public:
            struct Dependency{
                std::type_index type;

                // If marked optional. If the dependency is registred, it will initialize after it. If not, it will just skip it.
                bool optional = false;
            };

            virtual ~ISystem() = default;

            virtual void initialize(Engine& engine);
            virtual void postInitialize();

            virtual void preShutdown();
            virtual void shutdown();

            virtual void top();
            virtual void bottom();

            virtual void preEvent();
            virtual void event();
            virtual void postEvent();

            virtual void preUpdate();
            virtual void update();
            virtual void postUpdate();

            virtual void preRender();
            virtual void render();
            virtual void postRender();

            virtual const char* name() const = 0;
            virtual std::vector<Dependency> dependencies() const;

            virtual void dependencyShutdown(const std::type_index& dependency, const ISystem& system);
    };
}