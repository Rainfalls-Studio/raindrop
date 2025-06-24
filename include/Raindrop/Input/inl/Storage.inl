#pragma once

namespace Raindrop::Input{
    template<typename T>
    void StorageDefinition::declare(const std::string& name, std::function<T&(void)> accessor){
        declare(
            name,
            typeid(T),
            [accessor](void) -> void * {
                return static_cast<void*>(&accessor());
            }
        );
    }


    template<typename T>
    T& StorageDefinition::access(const std::string& member){
        return *static_cast<T*>(access(member));
    }


    template<typename T>
    T& StorageGroup::access(const std::string& member){
        return *static_cast<T*>(access(member));
    }


    template<typename T>
    T& Storage::getGroup(const std::string& name){
        return static_cast<T&>(getGroup(name));
    }

    template<typename T>
    T& Storage::access(const std::string& name){
        return *static_cast<T*>(access(name));
    }
}