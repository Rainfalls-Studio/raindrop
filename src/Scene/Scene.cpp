#include "Raindrop/Scene/Scene.hpp"
#include "Raindrop/Scene/Entity.hpp"
#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Scene{
    Scene::Scene(){}
    Scene::~Scene(){}

    Entity Scene::createEntity(){
        Entity entity = Entity(this, _registry.create());
        foreachBehavior([&entity](IBehavior& behavior){behavior.onCreate(entity);});
        return entity;
    }

    void Scene::destroy(Entity entity){
        foreachBehavior([&entity](IBehavior& behavior){behavior.onDestroy(entity);});
        _registry.destroy(entity.getHandle());
    }

    void Scene::addBehavior(const std::shared_ptr<IBehavior>& behavior){
        _owned.push_back(behavior);
        behavior->initialize(*this);
    }

    void Scene::addExternalBehavior(const std::shared_ptr<IBehavior>& behavior){
        _external.push_back(behavior);
        behavior->initialize(*this);
    }

    Entity Scene::getEntity(EntityHandle handle){
        return Entity(this, handle);
    }

    entt::basic_registry<EntityHandle>& Scene::getRegistry(){
        return _registry;
    }

    void Scene::initialize(){}
    void Scene::shutdown(){}

    void Scene::preUpdate(){
        foreachBehavior([](IBehavior& behavior){behavior.preUpdate();});
    }

    void Scene::update(){
        foreachBehavior([](IBehavior& behavior){behavior.update();});
    }

    void Scene::postUpdate(){
        foreachBehavior([](IBehavior& behavior){behavior.postUpdate();});
    }

    void Scene::preRender(){
        foreachBehavior([](IBehavior& behavior){behavior.preRender();});
    }

    void Scene::render(){
        foreachBehavior([](IBehavior& behavior){behavior.render();});
    }

    void Scene::postRender(){
        foreachBehavior([](IBehavior& behavior){behavior.postRender();});
    }

    bool Scene::isValid(EntityHandle handle) const{
        return _registry.valid(handle);
    }

    void Scene::foreachBehavior(const std::function<void(IBehavior&)>& callback){
        for (auto behavior : _owned){
            callback(*behavior);
        }

        auto it = _external.begin();
        while (it != _external.end()){
            auto lock = it->lock();

            if (!lock){
                it = _external.erase(it);
            } else {
                callback(*lock);
                ++it;
            }
        }
    }

}