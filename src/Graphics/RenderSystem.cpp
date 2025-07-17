#include "Raindrop/Graphics/RenderSystem.hpp"
#include "Raindrop/Graphics/Context.hpp"
#include "Raindrop/Graphics/GraphicsEngine.hpp"
#include "Raindrop/Window/WindowSystem.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Graphics{
    void RenderSystem::initialize(Raindrop::Engine& engine){
        _engine = std::make_unique<GraphicsEngine>();
        _engine->initialize(engine);
    }

    void RenderSystem::postInitialize(){
        _engine->postInitialize();
    }

    void RenderSystem::preShutdown(){
        _engine->preShutdown();
    }

    void RenderSystem::shutdown(){
        _engine->shutdown();
    }

    const char* RenderSystem::name() const{
        return "Renderer";
    }

    void RenderSystem::preRender(){
        _engine->preRender();
    }

    void RenderSystem::render(){
        _engine->render();
    }

    void RenderSystem::postRender(){
        _engine->postRender();
    }

    std::vector<RenderSystem::Dependency> RenderSystem::dependencies() const{
        return {
            Dependency{typeid(Raindrop::Window::WindowSystem), true}
        };
    }
}