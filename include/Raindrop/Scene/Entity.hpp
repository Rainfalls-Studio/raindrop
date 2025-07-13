#pragma once

#include "Scene.hpp"

namespace Raindrop::Scene{
    class Entity{
        public:
            Entity();
            Entity(Scene* scene, EntityHandle handle);

            Entity(Entity&& other);
            Entity(const Entity& other);

            Entity& operator=(Entity&& other);
            Entity& operator=(const Entity& other);

            Scene* getScene() const;
            EntityHandle getHandle() const;

            bool isValid() const;
            
            inline operator bool() const{
                return isValid();
            }

            template<typename... Components>
            inline decltype(auto) get(){
                return validateScene().getComponent<Components...>(_handle);
            }

            template<typename Component, typename... Args>
            inline decltype(auto) emplace(Args&&... args){
                return validateScene().emplaceComponent<Component, Args...>(_handle, std::forward<Args>(args)...);
            }

        private:
            Scene* _scene;
            EntityHandle _handle;

            Scene& validateScene();
    };
}