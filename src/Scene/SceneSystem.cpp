#include "Raindrop/Scene/SceneSystem.hpp"
#include "Raindrop/Scene/LayerModule.hpp"
#include "Raindrop/Layer/Layer.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Scene{
    void SceneSystem::initialize(Engine& engine){
        _engine = &engine;
    }

    void SceneSystem::postInitialize(){
        foreachLayer([](Scene& scene){scene.initialize();});
    }
    
    void SceneSystem::preShutdown(){
        foreachLayer([](Scene& scene){scene.shutdown();});
    }

    void SceneSystem::shutdown(){
        _engine = nullptr;
    }

    void SceneSystem::foreachLayer(const std::function<void(Scene&)>& func){
        for (auto handle : _layers){

            auto layer = _engine->getLayerManager().getLayer(handle);
            func(layer.getModules<LayerModule>().scene);
        }
    }

    void SceneSystem::top(){
        _layers = _engine->getLayerManager().view<LayerModule>();
    }

    void SceneSystem::bottom(){}

    void SceneSystem::preUpdate(){
        foreachLayer([](Scene& scene){scene.preUpdate();});
    }

    void SceneSystem::update(){
        foreachLayer([](Scene& scene){scene.update();});
    }

    void SceneSystem::postUpdate(){
        foreachLayer([](Scene& scene){scene.postUpdate();});
    }

    void SceneSystem::preRender(){
        foreachLayer([](Scene& scene){scene.preRender();});
    }

    void SceneSystem::render(){
        foreachLayer([](Scene& scene){scene.render();});
    }

    void SceneSystem::postRender(){
        foreachLayer([](Scene& scene){scene.postRender();});
    }

    const char* SceneSystem::name() const{
        return "Scene system";
    }
}