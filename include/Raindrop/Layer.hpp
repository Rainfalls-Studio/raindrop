#pragma once

#include "LayerRegistry.hpp"
#include <cstdint>

namespace Raindrop{
    class LayerRegistry;

    class Layer{
        public:
            Layer(LayerRegistry* registry, LayerID handle);

            template<typename... Components>
            inline decltype(auto) get(){
                return getRegistry().get<Components...>(_id);
            }

            template<typename Component, typename... Args>
            inline decltype(auto) add(Args&&... args){
                return getRegistry().emplace<Component>(_id, args...);
            }

            LayerRegistry& getRegistry();

        private:
            LayerRegistry* _registry;
            LayerID _id;
    };
}