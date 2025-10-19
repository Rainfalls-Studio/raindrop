#include "Raindrop/Modules/Window/Window.hpp"
#include <stdexcept>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace Raindrop::Window{
    SDL_WindowFlags raindropToSDLFlags(WindowFlags flags){
        SDL_WindowFlags out = 0;

        if (flags & WindowFlags::FULLSCREEN) out |= SDL_WINDOW_FULLSCREEN;
        if (flags & WindowFlags::BORDERLESS) out |= SDL_WINDOW_BORDERLESS;
        if (flags & WindowFlags::RESIZABLE) out |= SDL_WINDOW_RESIZABLE;
        if (flags & WindowFlags::UTILITY) out |= SDL_WINDOW_UTILITY;
        if (flags & WindowFlags::TRANSPARENT) out |= SDL_WINDOW_TRANSPARENT;
        if (flags & WindowFlags::NOT_FOCUSABLE) out |= SDL_WINDOW_NOT_FOCUSABLE;
        if (flags & WindowFlags::HIDDEN) out |= SDL_WINDOW_HIDDEN;

        return out;
    }

    void Window::resetFlags(){
        _resized = false;
    }


    Window::Window(const WindowConfig& config){
        _window = SDL_CreateWindow(
            config.title.c_str(),
            static_cast<int>(config.resolution.x),
            static_cast<int>(config.resolution.y),
            raindropToSDLFlags(config.flags) | SDL_WINDOW_VULKAN
        );

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

    std::expected<vk::SurfaceKHR, Error> Window::createSurface(vk::Instance instance){
        VkSurfaceKHR surface;
        if (SDL_Vulkan_CreateSurface(_window, instance, nullptr, &surface) == false){
            return std::unexpected(Error(std::make_error_code(std::errc::invalid_argument), SDL_GetError()));
        }

        return vk::SurfaceKHR(surface);
    }

    glm::u32vec2 Window::getResolution() const{
        int w, h;
        if (SDL_GetWindowSizeInPixels(_window, &w, &h) == false){
            throw std::runtime_error(SDL_GetError());
        }
        return glm::u32vec2{w, h};
    }

    std::string_view Window::getTitle() const{
        return std::string_view(SDL_GetWindowTitle(_window));
    }

    Display Window::getDisplay() const{
        SDL_DisplayID id = SDL_GetDisplayForWindow(_window);
        return Display(id);
    }
}