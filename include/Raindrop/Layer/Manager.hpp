#pragma once

#include "../Core/Utils/BehaviorRegistry.hpp"
#include "Raindrop/Engine.hpp"
#include <map>
#include <functional>
#include "Layer.hpp"

namespace Raindrop::Layer{
    class Manager : public Core::Utils::BehaviorRegistry<uint32_t, std::shared_ptr<Layer>>{
        using Super = Core::Utils::BehaviorRegistry<uint32_t, std::shared_ptr<Layer>>;
        
        public:
            Manager(Engine& engine);

            std::pair<ID, std::shared_ptr<Layer>> createLayer(float depth);

            void foreach(std::function<void(Layer&)> callback);
        
        private:
            Engine& _engine;
            std::map<float, ID> _layers;

            // Hide insert
            using Super::insert;
    };

    using LayerID = Manager::ID;
    static constexpr LayerID INVALID_LAYER_ID = Manager::INVALID_ID;
}