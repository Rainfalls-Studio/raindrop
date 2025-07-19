#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <string_view>

namespace Raindrop::Window{
    class Display{
        public:
            Display(SDL_DisplayID id);
            ~Display() = default;

            glm::u32vec2 getResolution() const;
            float getPixelDensity() const;
            float getRefreshRate() const;
            float getContentScale() const;
            std::string_view getName() const;

        private:
            SDL_DisplayID _id;
    };
}