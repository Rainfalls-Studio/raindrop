#include "Raindrop/Core/Layers/Manager.hpp"
#include "Raindrop/Core/Layers/Layer.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Layers{
    Manager::Manager(Engine &engine) : _engine{engine}{}
    Manager::~Manager(){}

    void Manager::shutdown(){
        spdlog::info("Shuting down layers...");
        _layers.clear();
    }


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