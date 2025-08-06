#include "Raindrop/Modules/Scene/Scene.hpp"
#include "Raindrop/Modules/Scene/Entity.hpp"
#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Scene{
    Scene::Scene(){}
    Scene::~Scene(){}

    void Scene::initialize(Engine& engine){
        _engine = &engine;
    }

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
        behavior->initialize(*_engine, *this);
    }

    void Scene::addExternalBehavior(const std::shared_ptr<IBehavior>& behavior){
        _external.push_back(behavior);
        behavior->initialize(*_engine, *this);
    }

    Entity Scene::getEntity(EntityHandle handle){
        return Entity(this, handle);
    }

    entt::basic_registry<EntityHandle>& Scene::getRegistry(){
        return _registry;
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