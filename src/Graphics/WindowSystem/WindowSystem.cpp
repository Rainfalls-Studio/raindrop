#include "Raindrop/Graphics/WindowSystem/WindowSystem.hpp"
#include "Raindrop/Graphics/Core/CoreSystem.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics::WindowSystem{
    void WindowSystem::initialize(GraphicsEngine& engine){
        createLogger();
        _engine = &engine;
    }

    void WindowSystem::createLogger(){
        _logger = spdlog::get("Raindrop::Graphics::Window");
        if (!_logger){
            _logger = spdlog::stdout_color_mt("Raindrop::Graphics::Window");
        }
    }

    void WindowSystem::postInitialize(){}

    void WindowSystem::shutdown(){
        _contexts.clear();
    }

    void WindowSystem::preRender(){
        auto sys = _windowSystem.lock();

        if (!sys){
            // if the system becomes invalid, we need to reset the systems
            if (!_contexts.empty()){
                _contexts.clear();
            }

            return;
        }

        // sys->getWindow();
    }

    const char* WindowSystem::name() const{
        return "Window system";
    }

    std::vector<WindowSystem::Dependency> WindowSystem::dependencies() const{
        return {
            Dependency::Make<Core::CoreSystem>()
        };
    }
}