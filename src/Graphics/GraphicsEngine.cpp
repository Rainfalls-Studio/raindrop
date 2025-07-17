#include "Raindrop/Graphics/GraphicsEngine.hpp"
#include "Raindrop/Graphics/Core/CoreSystem.hpp"
#include "Raindrop/Graphics/WindowSystem/WindowSystem.hpp"
#include "Raindrop/Window/WindowSystem.hpp"

namespace Raindrop::Graphics{

    void GraphicsEngine::initialize(Engine& engine){
        _engine = &engine;
        _manager = std::make_unique<System::Manager>();

        _manager->emplaceSystem<Core::CoreSystem>();
        
        try {
            engine.getSystemManager().getSystem<Window::WindowSystem>();
            _manager->emplaceSystem<WindowSystem::WindowSystem>();
        } catch (const std::out_of_range&){
        } catch (...){
            throw;
        }

        _manager->initializeSystems(*this);
    }

    void GraphicsEngine::postInitialize(){
        _manager->foreach([](System::IRenderSystem& sys){sys.postInitialize();});
    }

    void GraphicsEngine::preShutdown(){
        _manager->foreach([](System::IRenderSystem& sys){sys.preShutdown();});
    }

    void GraphicsEngine::shutdown(){
        _manager->shutdownSystems();
    }

    void GraphicsEngine::preRender(){
        _manager->foreach([](System::IRenderSystem& sys){sys.preRender();});
    }
    void GraphicsEngine::render(){
        _manager->foreach([](System::IRenderSystem& sys){sys.render();});
    }
    void GraphicsEngine::postRender(){
        _manager->foreach([](System::IRenderSystem& sys){sys.postRender();});
    }

    System::Manager& GraphicsEngine::getSystemManager(){
        if (_manager){
            return *_manager;
        } else {
            throw std::runtime_error("Graphics Engine not initialized");
        }
    }

    Engine& GraphicsEngine::getEngine(){
        if (_engine){
            return *_engine;
        } else {
            throw std::runtime_error("Graphics Engine not initialized");
        }
    }
}