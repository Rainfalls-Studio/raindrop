#include "Raindrop/Graphics/WindowSystem/WindowSystem.hpp"

namespace Raindrop::Graphics::WindowSystem{
    void WindowSystem::initialize(GraphicsEngine& context){
        
    }

    void WindowSystem::postInitialize(){

    }

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
}