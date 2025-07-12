#include "Raindrop/Engine.hpp"

namespace Raindrop{
    Engine::Engine(){}

    void Engine::run(){
        _systems.initializeSystems(*this);
        _systems.foreach([this](System::ISystem& sys){sys.postInitialize();});
        _running = true;

        while (_running){
            _systems.foreach([](System::ISystem& sys){sys.preEvent();});
            _systems.foreach([](System::ISystem& sys){sys.event();});
            _systems.foreach([](System::ISystem& sys){sys.postEvent();});

            _systems.foreach([](System::ISystem& sys){sys.preUpdate();});
            _systems.foreach([](System::ISystem& sys){sys.update();});
            _systems.foreach([](System::ISystem& sys){sys.postUpdate();});

            _systems.foreach([](System::ISystem& sys){sys.preRender();});
            _systems.foreach([](System::ISystem& sys){sys.render();});
            _systems.foreach([](System::ISystem& sys){sys.postRender();});
        }

        _systems.foreach([this](System::ISystem& sys){sys.preShutdown();});
        _systems.shutdownSystems();
    }

    void Engine::stop(){
        _running = false;
    }

    // =========== layers ==============

    Layer::Manager& Engine::getLayerManager() noexcept{
        return _layers;
    }

    System::Manager& Engine::getSystemManager() noexcept{
        return _systems;
    }
}