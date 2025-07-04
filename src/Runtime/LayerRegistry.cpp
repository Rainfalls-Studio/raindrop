#include "Raindrop/LayerRegistry.hpp"
#include "Raindrop/Layer.hpp"

namespace Raindrop{
    LayerRegistry::LayerRegistry(){
        
    }

    Layer LayerRegistry::createLayer(){
        LayerID id = _registry.create();
        _registry.emplace<LayerInfo>(id);

        return Layer(this, id);
    }
}