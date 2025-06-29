#include "Raindrop/Scene/Manager.hpp"

namespace Raindrop::Scene{
    Manager::Manager(Engine& engine) : _engine{engine}{}

    std::pair<Manager::ID, std::shared_ptr<Scene>> Manager::createScene(){
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        auto id = Super::insert(scene);

        return std::make_pair(id, scene);
    }
}