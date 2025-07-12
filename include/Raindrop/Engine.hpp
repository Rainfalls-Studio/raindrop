#pragma once

#include "Layer/Manager.hpp"
#include "System/Manager.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void run();
            void stop();

            Layer::Manager& getLayerManager() noexcept;
            System::Manager& getSystemManager() noexcept;

        private: 
            System::Manager _systems;
            Layer::Manager _layers;

            bool _running;
    };
}