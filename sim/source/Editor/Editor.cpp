#include "Editor.hpp"

namespace Editor{
    Editor::Editor(){

    }

    void Editor::registerComponent(const ComponentInfo& component){
        _components.push_back(component);
    }

    std::vector<std::shared_ptr<Raindrop::Scene::Scene>> Editor::scenes(){
        std::vector<std::shared_ptr<Raindrop::Scene::Scene>> out;

        for (auto scene : _scenes){
            if (auto lock = scene.lock()){
                out.push_back(lock);
            }
        }

        return out;
    }

    void Editor::registerScene(std::shared_ptr<Raindrop::Scene::Scene> scene){
        _scenes.push_back(scene);
    }

    const std::vector<ComponentInfo>& Editor::components() const{
        return _components;
    }


    Entity Editor::selected() const{
        return _selected;
    }

    void Editor::select(Entity entity){
        _selected = entity;
    }
}