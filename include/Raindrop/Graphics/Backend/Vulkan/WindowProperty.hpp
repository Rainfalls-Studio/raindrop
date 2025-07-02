#pragma once

#include "Raindrop/Graphics/Backend/Vulkan/WindowContext.hpp"
#include "Raindrop/Window/Property.hpp"
#include <memory>

namespace Raindrop::Graphics::Backend::Vulkan{
    class WindowProperty : public Raindrop::Window::Property{
        public:
            WindowProperty(Window::Window& _window) : Raindrop::Window::Property(_window){}

            std::unique_ptr<WindowContext> context;
            
    };
}