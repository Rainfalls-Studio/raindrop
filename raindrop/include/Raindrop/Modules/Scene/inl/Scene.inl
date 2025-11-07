#pragma once
#include "../Scene.hpp"

namespace Raindrop::Scene{
    template<typename... Components>
    inline decltype(auto) Scene::getComponent(EntityHandle handle){
        return _registry.get<Components...>(handle);
    }

    template<typename Components, typename... Args>
    inline decltype(auto) Scene::emplaceComponent(EntityHandle handle, Args&&... args){
        return _registry.emplace<Components, Args...>(handle, std::forward<Args>(args)...);
    }

    template<typename... Components>
    inline bool Scene::hasAnyComponents(EntityHandle handle) const {
        return _registry.any_of<Components...>(handle);
    }

    template<typename... Components>
    inline bool Scene::hasAllComponents(EntityHandle handle) const {
        return _registry.all_of<Components...>(handle);
    }

    template<typename T>
    inline T& Scene::getBehavior(BehaviorID id){
        return reinterpret_cast<T&>(getBehavior(id));
    }

    template<typename T>
    inline T& Scene::getBehaviorFromType(){
        return reinterpret_cast<T&>(getBehaviorFromType(typeid(T)));
    }
}