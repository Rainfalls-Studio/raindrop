#pragma once

#include <entt/entt.hpp>

namespace Raindrop{
    using LayerID = uint32_t;
    class Layer;

    struct LayerInfo{
        std::string name;
    };

    class LayerRegistry{
        public:
            LayerRegistry();

            Layer createLayer();

            template<typename... Components>
            inline decltype(auto) get(LayerID id){
                return _registry.get<Components...>(id);
            }

            template<typename Component, typename... Args>
            inline decltype(auto) emplace(LayerID id, Args&&... args){
                return _registry.emplace<Component>(id, args...);
            }

        private:
            entt::basic_registry<LayerID> _registry;
    };
}