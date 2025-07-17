#include "Raindrop/Window/Window.hpp"
#include <stdexcept>
#include <SDL3/SDL_vulkan.h>

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
    
    std::vector<std::string_view> Window::getInstanceExtensions(){
        uint32_t count;
        auto ext_char = SDL_Vulkan_GetInstanceExtensions(&count);
        
        std::vector<std::string_view> exts(count);
        for (uint32_t i=0; i<count; i++){
            exts[i] = ext_char[i];
        }
        
        return exts;

    }

    vk::SurfaceKHR Window::createSurface(vk::Instance instance){
        VkSurfaceKHR surface;
        if (SDL_Vulkan_CreateSurface(_window, instance, nullptr, &surface) == false){
            throw std::runtime_error("Failed to create surface : " + std::string(SDL_GetError()));
        }

        return vk::SurfaceKHR(surface);
    }
}