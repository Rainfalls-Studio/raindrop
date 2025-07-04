#pragma once

#include <entt/entt.hpp>

namespace Raindrop::Scene{
    using EntityID = uint32_t;

    struct EntityInfo{
        std::string name;
        EntityID parent;
        std::vector<EntityID> children;
    };

    class Entity;
    class Scene{
        public:
            Scene();
            ~Scene();

            Entity create();

        private:
            entt::basic_registry<EntityID> _registry;
    };
}