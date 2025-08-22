#include <iostream>
#include "Raindrop/Engine.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop{
    Engine::Engine() : 
        _modules(*this),
        _layers(*this),
        _scheduler(*this),
        _store(*this),
        _tasks(*this)
    {
        spdlog::info("===================== Startup =====================");
    }

    void Engine::start(){
        spdlog::info("===================== Starting mainloop =====================");

        _tasks.workerLoop();
        
        spdlog::info("===================== Shutdown =====================");
        _scheduler.shutdown();
        _tasks.shutdown();
        _store.shutdown();
        _layers.shutdown();
        _modules.shutdown();
    }

    void Engine::stop(){
        _tasks.stop();
    }

    Modules::Manager& Engine::getModuleManager() noexcept{
        return _modules;
    }

    Layers::Manager& Engine::getLayerManager() noexcept{
        return _layers;
    }

    Scheduler::Scheduler& Engine::getScheduler() noexcept{
        return _scheduler;
    }

    Store::Store& Engine::getStore() noexcept{
        return _store;
    }

    Tasks::TaskManager& Engine::getTaskManager() noexcept{
        return _tasks;
    }
}