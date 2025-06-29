#pragma once

#include "Raindrop/Layer/Behavior.hpp"
#include "Raindrop/Layer/Layer.hpp"
#include "Raindrop/Layer/Manager.hpp"

namespace Raindrop::Scene{
    class LayerBehavior : public Layer::Behavior{
        public:
            virtual ~LayerBehavior() override = default;

            virtual void onCreation(Layer::LayerID id, const std::shared_ptr<Layer::Layer>& layer) override;
            virtual void onDestruction(Layer::LayerID id, const std::shared_ptr<Layer::Layer>& layer) override;
    };
}