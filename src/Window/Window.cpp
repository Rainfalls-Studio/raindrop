#include "Raindrop/Window/Window.hpp"
#include "Raindrop/Window/Window.hpp"
#include "spdlog/spdlog.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdexcept>


namespace Raindrop::Window{
    Window::Window(Engine& engine) : _engine{engine}, _handle{nullptr}{
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0){
            SPDLOG_TRACE("Initializing SDL3 ...");

            if (!SDL_Init(SDL_INIT_VIDEO)){
                SPDLOG_ERROR("Failed to initialize SDL3 : %s", SDL_GetError());
                throw std::runtime_error("Failed to initialize SDL3");
            }

            SPDLOG_TRACE("SDL3 initialized successfully");
        }

        SPDLOG_TRACE("Creating SDL3 window ...");

        _handle = SDL_CreateWindow(
            "Raindrop window",
            800,
            600,
            SDL_WINDOW_VULKAN
        );

        if (!_handle){
            SPDLOG_ERROR("Failed to create SDL3 window : %s", SDL_GetError());
            throw std::runtime_error("Failed to create window");
        }

        SPDLOG_TRACE("SDL3 window created succsessfully");
    }

    Window::~Window(){
        if (_handle){
            SPDLOG_TRACE("Destroying SDL3 Window");
            SDL_DestroyWindow(static_cast<SDL_Window*>(_handle));
        }
        
        int count;
        SDL_GetWindows(&count);

        if (count == 0){
            SPDLOG_TRACE("Shutting down SDL3 ...");
            SDL_Quit();
        }
    }

    glm::ivec2 Window::getSize() const{
        int w, h;
        SDL_GetWindowSizeInPixels(static_cast<SDL_Window*>(_handle), &w, &h);
        return glm::ivec2(w, h);
    }

    glm::ivec2 Window::getPosition() const{
        int x, y;
        SDL_GetWindowPosition(static_cast<SDL_Window*>(_handle), &x, &y);
        return glm::ivec2(x, y);
    }

    const char* Window::getTitle() const{
        return SDL_GetWindowTitle(static_cast<SDL_Window*>(_handle));
    }

    bool Window::isFullscreen() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_FULLSCREEN;
    }

    bool Window::isMinimized() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_MINIMIZED;
    }

    bool Window::isMaximized() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_MAXIMIZED;
    }

    bool Window::resized() const{
        return false;
    }

    bool Window::moved() const{
        return false;
    }
    
    void Window::setSize(glm::ivec2 size){
        SDL_SetWindowSize(static_cast<SDL_Window*>(_handle), size.x, size.y);
    }

    void Window::setPosition(glm::ivec2 position){
        SDL_SetWindowPosition(static_cast<SDL_Window*>(_handle), position.x, position.y);
    }

    void* Window::getNativeHandle() const{
        return _handle;
    }
}