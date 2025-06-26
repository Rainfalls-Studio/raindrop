#pragma once

#include "Raindrop/Engine.hpp"
#include <map>
#include <functional>
#include "Layer.hpp"

namespace Raindrop::Layer{
    class Manager{
        public:
            Manager(Engine& engine);

            std::shared_ptr<Layer> addLayer(float level);

            void foreach(std::function<void(Layer&)> callback);

        private:
            Engine& _engine;
            std::map<float, std::shared_ptr<Layer>> _layers;
    };
}