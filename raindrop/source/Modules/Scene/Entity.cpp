#include "Raindrop/Modules/Scene/Entity.hpp"

namespace Raindrop::Scene{
    Entity::Entity() : 
        _scene{nullptr},
        _handle{INVALID_ENTITY_HANDLE}
    {}

    Entity::Entity(Scene* scene, EntityHandle handle) : 
        _scene{scene},
        _handle{handle}
    {}

    Entity::Entity(Entity&& other) : 
        _scene{std::move(other._scene)},
        _handle{std::move(other._handle)}
    {}

    Entity::Entity(const Entity& other) : 
        _scene{other._scene},
        _handle{other._handle}
    {}

    Entity& Entity::operator=(Entity&& other){
        _scene = std::move(other._scene);
        _handle = std::move(other._handle);
        return *this;
    }

    Entity& Entity::operator=(const Entity& other){
        _scene = other._scene;
        _handle = other._handle;
        return *this;
    }

    Scene* Entity::getScene() const{
        return _scene;
    }

    EntityHandle Entity::getHandle() const{
        return _handle;
    }

    Scene& Entity::validateScene(){
        assert(_scene != nullptr);
        return *_scene;
    }

    const Scene& Entity::validateScene() const{
        assert(_scene != nullptr);
        return *_scene;
    }


    bool Entity::isValid() const{
        if (_handle == INVALID_ENTITY_HANDLE || _scene == nullptr) return false;
        return _scene->isValid(_handle);
    }

    bool Entity::operator==(const Entity& other) const{
        return _handle == other._handle && _scene == other._scene;
    }

}