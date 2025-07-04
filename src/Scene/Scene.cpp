#include "Raindrop/Scene/Scene.hpp"
#include "Raindrop/Scene/Entity.hpp"

namespace Raindrop::Scene{
    Scene::Scene(){
        
    }

    Scene::~Scene(){
        
    }

    Entity Scene::create(){
        EntityID id = _registry.create();
        _registry.emplace<EntityInfo>(id);
        return Entity(this, id);
    }
}