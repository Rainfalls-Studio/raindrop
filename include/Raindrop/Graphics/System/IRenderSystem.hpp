#pragma once

#include <typeindex>
#include <vector>

namespace Raindrop::Graphics{
    class GraphicsEngine;

    namespace System{
        class IRenderSystem{
            public:
                struct Dependency{
                    inline constexpr Dependency(const std::type_index& _type, bool _optional = false) :
                        type{_type},
                        optional{_optional}
                    {}

                    template<typename T>
                    inline static Dependency Make(bool _optional = false){
                        return Dependency(typeid(T), _optional);
                    } 

                    std::type_index type;

                    // If marked optional. If the dependency is registred, it will initialize after it. If not, it will just skip it.
                    bool optional = false;
                };

                virtual ~IRenderSystem() = default;

                virtual void initialize(GraphicsEngine& engine);
                virtual void postInitialize();
                virtual void preShutdown();
                virtual void shutdown();

                virtual void preRender();
                virtual void render();
                virtual void postRender();

                virtual std::vector<Dependency> dependencies() const;
                virtual const char* name() const = 0;
                virtual void dependencyShutdown(const std::type_index& dependency, const IRenderSystem& system);
        };
    }
}