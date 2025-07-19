#include "Raindrop/Window/Display.hpp"
#include <stdexcept>

namespace Raindrop::Window{
    Display::Display(SDL_DisplayID id) : _id{id}{}

    glm::u32vec2 Display::getResolution() const{
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(_id);

        if (!mode){
            throw std::runtime_error(SDL_GetError());
        }

        return glm::u32vec2{
            static_cast<uint32_t>(mode->w),
            static_cast<uint32_t>(mode->h)
        };
    }

    float Display::getPixelDensity() const{
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(_id);

        if (!mode){
            throw std::runtime_error(SDL_GetError());
        }

        return mode->pixel_density;
    }

    float Display::getRefreshRate() const{
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(_id);

        if (!mode){
            throw std::runtime_error(SDL_GetError());
        }

        return static_cast<int>(mode->refresh_rate) != 0 ? mode->refresh_rate : 60.f;
    }

    float Display::getContentScale() const{
        float scale = SDL_GetDisplayContentScale(_id);

        if (static_cast<int>(scale) == 0){
            throw std::runtime_error(SDL_GetError());
        }

        return scale;
    }

    std::string_view Display::getName() const{
        return std::string_view(SDL_GetDisplayName(_id));
    }
}