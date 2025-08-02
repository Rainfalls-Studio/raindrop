#include "Raindrop/Engine.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop{
    Engine::Engine() : 
        _modules(*this),
        _layers(*this),
        _scheduler(*this)
    {}

    void Engine::start(){
        spdlog::info("Starting mainloop...");

        _running = true;
        while (_running){
            _scheduler.trigger();
        }

        _modules.shutdown();
    }

    void Engine::stop(){
        _running = false;
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
}