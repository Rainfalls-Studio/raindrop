#include "Raindrop/Graphics/WindowSystem/WindowSystem.hpp"
#include "Raindrop/Graphics/GraphicsEngine.hpp"
#include "Raindrop/Graphics/Core/CoreSystem.hpp"
#include "Raindrop/Graphics/Output/OutputSystem.hpp"
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
        if (_window){
            destroyContext(*_window);
            _window.reset();
        }
    }

    void WindowSystem::registerWindow(std::shared_ptr<Window::Window> window){
        assert(window);

        if (_window){
            SPDLOG_LOGGER_WARN(_logger, "A window is already registred, overwriting it");
        }

        _window = std::make_shared<WindowContext>();

        _window->window = window;
        buildContext(*_window);

        auto outputSystem = _engine->getSystemManager().getSystem<Output::OutputSystem>();
        if (outputSystem){
            _window->target = std::make_shared<WindowOutputTarget>(_window);
            outputSystem->setOutput<WindowOutputTarget>(_window->target);
        }
    }

    void WindowSystem::buildContext(WindowContext& ctx){
        auto coreRef = _engine->getSystemManager().getSystem<Core::CoreSystem>();
        auto window = ctx.window.lock();

        ctx.surface = window->createSurface(coreRef->getInstance());
        ctx.capabilities = coreRef->getPhysicalDevice().getSurfaceCapabilitiesKHR(ctx.surface);
    }

    void WindowSystem::destroyContext(WindowContext& ctx){
        auto coreRef = _engine->getSystemManager().getSystem<Core::CoreSystem>();
        auto instance = coreRef->getInstance();

        instance.destroySurfaceKHR(ctx.surface);
    }


    void WindowSystem::preRender(){
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