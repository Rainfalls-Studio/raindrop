#pragma once

#include <memory>
#include "Core/Modules/Manager.hpp"
#include "Core/Layers/Manager.hpp"
#include "Core/Scheduler/Scheduler.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void start();
            void stop();

            Modules::Manager& getModuleManager() noexcept;
            Layers::Manager& getLayerManager() noexcept;
            Scheduler::Scheduler& getScheduler() noexcept;

        private:
            Modules::Manager _modules;
            Layers::Manager _layers;
            Scheduler::Scheduler _scheduler;

            bool _running;
    };
}