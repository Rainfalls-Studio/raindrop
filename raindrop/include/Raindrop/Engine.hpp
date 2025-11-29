#pragma once

#include "Modules/Manager.hpp"
#include "Scheduler/Scheduler.hpp"
#include "Tasks/TaskManager.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void start();
            void stop();

            Modules::Manager& getModuleManager() noexcept;
            Scheduler::Scheduler& getScheduler() noexcept;
            Tasks::TaskManager& getTaskManager() noexcept;

        private:
            Modules::Manager _modules;
            Scheduler::Scheduler _scheduler;
            Tasks::TaskManager _tasks;
    };
}