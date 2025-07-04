#include "Raindrop/Layer.hpp"
#include <stdexcept>

namespace Raindrop{
    Layer::Layer(LayerRegistry* registry, LayerID id) : 
        _registry{registry},
        _id{id}
    {}

    LayerRegistry& Layer::getRegistry(){
        if (!_registry){
            throw std::runtime_error("Registry not set for layer");
        }
        return *_registry;
    }
}