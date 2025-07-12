#include "Raindrop/Layer/Manager.hpp"
#include "Raindrop/Layer/Layer.hpp"

namespace Raindrop::Layer{
    Manager::Manager(){}
    Manager::~Manager(){}

    Layer Manager::createLayer(){
        return Layer{this, _layers.create()};
    }

    Layer Manager::getLayer(LayerHandle handle){
        return Layer{this, handle};
    }

    entt::basic_registry<LayerHandle>& Manager::getRegistry() noexcept{
        return _layers;
    }
}