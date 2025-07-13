#include "Raindrop/Scene/Scene.hpp"
#include "Raindrop/Scene/Entity.hpp"

namespace Raindrop::Scene{
    Scene::Scene(){}
    Scene::~Scene(){}

    Entity Scene::createEntity(){
        return Entity(this, _registry.create());
    }

    void Scene::destroy(Entity entity){
        _registry.destroy(entity.getHandle());
    }

    void Scene::addBehavior(const std::shared_ptr<IBehavior>& behavior){
        _owned.push_back(behavior);
    }

    void Scene::addExternalBehavior(const std::shared_ptr<IBehavior>& behavior){
        _external.push_back(behavior);
    }

    void Scene::preUpdate(){

    }

    void Scene::update(){

    }

    void Scene::postUpdate(){

    }

    void Scene::preRender(){

    }

    void Scene::render(){

    }

    void Scene::postRender(){

    }

    bool Scene::isValid(EntityHandle handle) const{
        return _registry.valid(handle);
    }
}