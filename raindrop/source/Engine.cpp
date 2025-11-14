#include <iostream>
#include "Raindrop/Engine.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop{
    void printTitle(const char* title){
        spdlog::info("----------------------------------------------------------");
        spdlog::info("                         {}                           ", title);
        spdlog::info("----------------------------------------------------------");
    }

    Engine::Engine() : 
        _modules(*this),
        _scheduler(*this),
        _tasks(*this, 2)
    {
        printTitle("Startup");
    }

    void Engine::start(){
        printTitle("Runtime");

        _tasks.workerLoop();
        
        printTitle("Shutdown");

        _tasks.shutdown();
        _scheduler.shutdown();
        _modules.shutdown();

        printTitle("Stopped");
    }

    void Engine::stop(){
        _tasks.stop();
    }

    Modules::Manager& Engine::getModuleManager() noexcept{
        return _modules;
    }

    Scheduler::Scheduler& Engine::getScheduler() noexcept{
        return _scheduler;
    }

    Tasks::TaskManager& Engine::getTaskManager() noexcept{
        return _tasks;
    }
}