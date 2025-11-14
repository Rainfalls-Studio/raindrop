#pragma once

#include "Core/Modules/Manager.hpp"
#include "Core/Scheduler/Scheduler.hpp"
#include "Core/Tasks/TaskManager.hpp"

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