#include "Raindrop/Scene/LayerBehavior.hpp"
#include "Raindrop/Scene/LayerProperty.hpp"
#include <iostream>

namespace Raindrop::Scene{
    void LayerBehavior::onCreation(Layer::LayerID, const std::shared_ptr<Layer::Layer>& layer){
        std::shared_ptr<LayerProperty> property = std::make_shared<LayerProperty>();
        layer->addProperty(property);
    }

    void LayerBehavior::onDestruction(Layer::LayerID, const std::shared_ptr<Layer::Layer>& layer){
        layer->removeProperty<LayerProperty>();
    }

}