#pragma once

#include <cstdint>
#include <entt/entt.hpp>

#include "Raindrop/Engine.hpp"

namespace Raindrop::Scene{
    using EntityHandle = uint32_t;
    static constexpr EntityHandle INVALID_ENTITY_HANDLE = ~static_cast<EntityHandle>(0);

    class Entity;
    class IBehavior;

    class Scene{
        public:
            Scene();
            ~Scene();

            void initialize(Engine& engine);

            Entity createEntity();
            void destroy(Entity entity);

            Entity getEntity(EntityHandle handle);

            bool isValid(EntityHandle handle) const;

            template<typename... Components>
            inline decltype(auto) getComponent(EntityHandle handle){
                return _registry.get<Components...>(handle);
            }

            template<typename Components, typename... Args>
            inline decltype(auto) emplaceComponent(EntityHandle handle, Args&&... args){
                return _registry.emplace<Components, Args...>(handle, std::forward<Args>(args)...);
            }

            template<typename T, typename... Args>
            std::shared_ptr<T> emplaceBehavior(Args&&... args){
                std::shared_ptr<T> behavior = std::make_shared<T>(std::forward<Args>(args)...);
                addBehavior(behavior);
                return behavior;
            }

            template<typename T, typename... Others>
            inline decltype(auto) view(){
                return _registry.view<T, Others...>();
            }

            void addBehavior(const std::shared_ptr<IBehavior>& behavior);
            void addExternalBehavior(const std::shared_ptr<IBehavior>& behavior);

            entt::basic_registry<EntityHandle>& getRegistry();

        private:
            Engine* _engine;
            entt::basic_registry<EntityHandle> _registry;

            std::list<std::shared_ptr<IBehavior>> _owned;
            std::list<std::weak_ptr<IBehavior>> _external;

            void foreachBehavior(const std::function<void(IBehavior&)>& callback);
    };
}