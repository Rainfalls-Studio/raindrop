#include "Raindrop/Engine.hpp"

#include <spdlog/spdlog.h>

namespace Raindrop{
    Engine::Engine() : 
        _systems(*this),
        _layers(*this),
        _scheduler(*this)
    {}

    void Engine::start(){
        if (auto result = _systems.initialize(); !result){
            auto& error = result.error(); 
            spdlog::error("Failed to initialize systems with error {} : {}", error.message(), error.reason());
            return;
        }

        _running = true;
        while (_running){
            _scheduler.trigger();
        }

        _systems.shutdown();
    }

    void Engine::stop(){
        _running = false;
    }

    Systems::Manager& Engine::getSystemManager() noexcept{
        return _systems;
    }

    Layers::Manager& Engine::getLayerManager() noexcept{
        return _layers;
    }

    Scheduler::Scheduler& Engine::getScheduler() noexcept{
        return _scheduler;
    }
}