#include "Raindrop/Core/Layers/Manager.hpp"
#include "Raindrop/Core/Layers/Layer.hpp"

namespace Raindrop::Layers{
    Manager::Manager(Engine &engine) : _engine{engine}{}
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

    bool Manager::isValid(LayerHandle handle) const{
        return _layers.valid(handle);
    }
}