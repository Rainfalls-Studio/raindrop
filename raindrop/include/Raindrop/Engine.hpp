#pragma once

#include "Modules/Manager.hpp"
#include "Scheduler/Scheduler.hpp"
#include "Tasks/TaskManager.hpp"

#include <filesystem>

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void start();
            void stop();

            Modules::Manager& getModuleManager() noexcept;
            Scheduler::Scheduler& getScheduler() noexcept;
            Tasks::TaskManager& getTaskManager() noexcept;

            std::filesystem::path executableDirectory();

        private:
            Modules::Manager _modules;
            Scheduler::Scheduler _scheduler;
            Tasks::TaskManager _tasks;
    };
}