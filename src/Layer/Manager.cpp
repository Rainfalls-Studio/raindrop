#include "Raindrop/Layer/Manager.hpp"

namespace Raindrop::Layer{
    Manager::Manager(Engine& engine) : _engine{engine}{}

    std::pair<Manager::ID, std::shared_ptr<Layer>> Manager::createLayer(float depth){
        std::shared_ptr<Layer> layer = std::make_shared<Layer>();

        ID id = Super::insert(layer);
        _layers[depth] = id;

        return std::make_pair(id, layer);
    }


    void Manager::foreach(std::function<void(Layer&)> callback){
        for (auto it : _layers){
            auto layerID = it.second;

            auto layer = get(layerID);
            callback(*layer);
        }
    }
}