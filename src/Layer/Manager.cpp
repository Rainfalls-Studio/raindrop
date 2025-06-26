#include "Raindrop/Layer/Manager.hpp"

namespace Raindrop::Layer{
    Manager::Manager(Engine& engine) : _engine{engine}{}

    std::shared_ptr<Layer> Manager::addLayer(float level){
        std::shared_ptr<Layer> layer = std::make_shared<Layer>();
        _layers[level] = layer;
        return layer;
    }

    void Manager::foreach(std::function<void(Layer&)> callback){
        for (auto it : _layers){
            auto layer = it.second;
            callback(*layer);
        }
    }
}