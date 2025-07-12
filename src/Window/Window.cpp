#include "Raindrop/Window/Window.hpp"
#include <stdexcept>

namespace Raindrop::Window{

    Window::Window(){
        _window = SDL_CreateWindow("window", 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

        if (!_window){
            throw std::runtime_error("Failed to create SDL window : " + std::string(SDL_GetError()));
        }
    }

    Window::~Window(){
        if (_window){
            SDL_DestroyWindow(_window);
        }
    }

    SDL_Window* Window::getWindow() const{
        return _window;
    }

}