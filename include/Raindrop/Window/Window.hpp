#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string_view>
#include <vulkan/vulkan.hpp>

namespace Raindrop::Window{
    class Window{
        public:
            Window();
            ~Window();

            SDL_Window* getWindow() const;

			std::vector<std::string_view> getInstanceExtensions();
			vk::SurfaceKHR createSurface(vk::Instance instance);

        private:
            SDL_Window* _window;
    };
}