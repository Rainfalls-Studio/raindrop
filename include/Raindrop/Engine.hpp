#pragma once

#include <memory>
#include "Core/Systems/Manager.hpp"
#include "Core/Layers/Manager.hpp"
#include "Core/Scheduler/Scheduler.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void start();
            void stop();

            Systems::Manager& getSystemManager() noexcept;
            Layers::Manager& getLayerManager() noexcept;
            Scheduler::Scheduler& getScheduler() noexcept;

        private:
            Systems::Manager _systems;
            Layers::Manager _layers;
            Scheduler::Scheduler _scheduler;

            bool _running;
    };
}