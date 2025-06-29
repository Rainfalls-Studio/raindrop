#pragma once

#include "../Layer/Property.hpp"
#include "Raindrop/Scene/Manager.hpp"

namespace Raindrop::Scene{
    class LayerProperty : public Layer::Property{
        public:
            virtual ~LayerProperty() override = default;

            std::shared_ptr<Scene> scene;
    };
}