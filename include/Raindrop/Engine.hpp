#pragma once

#include "Core/Modules/Manager.hpp"
#include "Core/Layers/Manager.hpp"
#include "Core/Scheduler/Scheduler.hpp"
#include "Core/Store/Store.hpp"
#include "Core/Tasks/TaskManager.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void start();
            void stop();

            Modules::Manager& getModuleManager() noexcept;
            Layers::Manager& getLayerManager() noexcept;
            Scheduler::Scheduler& getScheduler() noexcept;
            Store::Store& getStore() noexcept;
            Tasks::TaskManager& getTaskManager() noexcept;

        private:
            Modules::Manager _modules;
            Layers::Manager _layers;
            Scheduler::Scheduler _scheduler;
            Store::Store _store;
            Tasks::TaskManager _tasks;

            bool _running;
    };
}