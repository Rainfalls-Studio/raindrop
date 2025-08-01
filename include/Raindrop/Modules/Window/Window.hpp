#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string_view>
#include <vulkan/vulkan.hpp>

#include "Raindrop/Core/Error.hpp"
#include "WindowConfig.hpp"
#include "Display.hpp"

namespace Raindrop::Window{
    class Window{
        public:
            Window(const WindowConfig& config);
            ~Window();

            SDL_Window* getWindow() const;

            glm::u32vec2 getResolution() const;
            std::string_view getTitle() const;
            Display getDisplay() const;

			std::vector<std::string_view> getInstanceExtensions();
			std::expected<vk::SurfaceKHR, Error> createSurface(vk::Instance instance);

        private:
            SDL_Window* _window;
            SDL_WindowID _id;
    };
}