#pragma once

#include <cstdint>
#include <entt/entt.hpp>
#include "IBehavior.hpp"

namespace Raindrop::Scene{
    using EntityHandle = uint32_t;
    static constexpr EntityHandle INVALID_ENTITY_HANDLE = ~static_cast<EntityHandle>(0);

    class Entity;

    class Scene{
        public:
            Scene();
            ~Scene();

            Entity createEntity();
            void destroy(Entity entity);

            bool isValid(EntityHandle handle) const;

            template<typename... Components>
            inline decltype(auto) getComponent(EntityHandle handle){
                return _registry.get<Components...>(handle);
            }

            template<typename Components, typename... Args>
            inline decltype(auto) emplaceComponent(EntityHandle handle, Args&&... args){
                return _registry.get<Components, Args...>(handle, std::forward<Args>(args)...);
            }

            template<typename T, typename... Args>
            std::shared_ptr<T> emplaceBehavior(Args&&... args){
                std::shared_ptr<T> behavior = std::make_shared<T>(std::forward<Args>(args)...);
                addBehavior(behavior);
                return behavior;
            }

            void preUpdate();
            void update();
            void postUpdate();

            void preRender();
            void render();
            void postRender();

            void addBehavior(const std::shared_ptr<IBehavior>& behavior);
            void addExternalBehavior(const std::shared_ptr<IBehavior>& behavior);

        private:
            entt::basic_registry<EntityHandle> _registry;

            std::list<std::shared_ptr<IBehavior>> _owned;
            std::list<std::weak_ptr<IBehavior>> _external;
    };
}