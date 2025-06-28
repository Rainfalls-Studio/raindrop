#pragma once

#include "../ResourceManager.hpp"
#include <memory>

namespace Raindrop::Graphics::ResourceManager{
    template<typename T, typename... Args>
    ResourceID ResourceManager::insertResource(const std::string& name, Args... args){
        checkResourceType<T>();
        return insertResource(name, typeid(T), std::static_pointer_cast<ResourceDefinition>(std::make_shared<T>(args...)));
    }

    template<typename T>
    ResourceID ResourceManager::insertResource(const std::string& name){
        checkResourceType<T>();
        return insertResource(name, typeid(T), std::static_pointer_cast<ResourceDefinition>(std::make_shared<T>()));
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::getResourceFromID(ResourceID ID){
        checkResourceType<T>();
        return std::static_pointer_cast<T>(getResourceFromID(ID));
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::getResourceFromName(const std::string& name){
        checkResourceType<T>();
        return std::static_pointer_cast<T>(getResourceFromName(name));
    }

    template<typename T>
    constexpr inline void ResourceManager::checkResourceType(){
        static_assert(std::is_base_of<ResourceDefinition, T>::value, "T must be derived from ResourceDefinition");
    }
}