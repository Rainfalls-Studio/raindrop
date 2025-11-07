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

    BehaviorID Scene::getBehaviorIndex(std::type_index type){
        auto it = _typeToIndex.find(type);
        if (it == _typeToIndex.end()) return INVALID_BEHAVIOR_ID;
        return it->second;
    }

    BehaviorID Scene::addBehavior(std::type_index index, std::shared_ptr<IBehavior>&& behavior){

        BehaviorID id = INVALID_BEHAVIOR_ID;

        // if there are no free IDs
        if (_freeBehaviorIDs.empty()){
            id = static_cast<BehaviorID>(_behaviors.size());
            _behaviors.emplace_back(std::move(behavior));

        } else {
            id = _freeBehaviorIDs.front();
            _freeBehaviorIDs.pop_front();
            _behaviors[id] = std::move(behavior);
        }

        _typeToIndex[index] = id;
        return id;
    }

    bool Scene::hasBehavior(std::type_index type) const{
        auto it = _typeToIndex.find(type);
        return it != _typeToIndex.end();
    }

    std::shared_ptr<IBehavior> Scene::getBehavior(BehaviorID id){
        return _behaviors[id];
    }

    std::shared_ptr<IBehavior> Scene::getBehaviorFromType(std::type_index type){
        auto id = getBehaviorIndex(type);
        return getBehavior(id);
    }

    Entity Scene::getEntity(EntityHandle handle){
        return Entity(this, handle);
    }

    entt::basic_registry<EntityHandle>& Scene::registry(){
        return _registry;
    }

    bool Scene::isValid(EntityHandle handle) const{
        return _registry.valid(handle);
    }

    void Scene::foreachBehavior(const std::function<void(IBehavior&)>& callback){
        for (auto behavior : _behaviors){
            callback(*behavior);
        }
    }


    StageID Scene::createStage(std::string_view name){
        StageID id = INVALID_STAGE_ID;

        if (_freeStageID.empty()){
            id = static_cast<StageID>(_stages.size());
            _stages.emplace_back(StageContent{{}, std::string(name)});
        } else {
            id = _freeStageID.front();
            _freeStageID.pop_front();
            _stages[id] = StageContent{{}, std::string(name)};
        }

        return id;
    }
    
    void Scene::destroyStage(StageID id){
        _freeStageID.push_back(id);
    }

    std::string_view Scene::stageName(StageID id){
        return _stages[id].name;
    }

    void Scene::executeStage(StageID id){
        auto& content = _stages[id];

        for (auto behaviorId : content.behaviors){
            auto& behavior = _behaviors[behaviorId];
            
            if (behavior){
                behavior->execute();
            }
        }
    }

    void Scene::addToStage(StageID stage, BehaviorID behavior){
        _stages[stage].behaviors.push_back(behavior);
    }

}