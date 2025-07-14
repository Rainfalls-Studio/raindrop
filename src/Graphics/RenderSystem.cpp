#include "Raindrop/Graphics/RenderSystem.hpp"
#include "Raindrop/Window/WindowSystem.hpp"

namespace Raindrop::Graphics{
    RenderSystem::RenderSystem(){

    }

    void RenderSystem::initialize(Engine&){

    }

    const char* RenderSystem::name() const{
        return "Renderer";
    }

    std::vector<RenderSystem::Dependency> RenderSystem::dependencies() const{
        return {
            Dependency{typeid(Raindrop::Window::WindowSystem), true}
        };
    }
}