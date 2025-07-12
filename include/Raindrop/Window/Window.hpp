#pragma once

#include <SDL3/SDL.h>

namespace Raindrop::Window{
    class Window{
        public:
            Window();
            ~Window();

            SDL_Window* getWindow() const;

        private:
            SDL_Window* _window;
    };
}