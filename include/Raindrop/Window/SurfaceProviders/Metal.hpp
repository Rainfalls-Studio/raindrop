#pragma once

#include "../SurfaceProvider.hpp"
#include <string_view>

typedef void *SDL_MetalView;

namespace Raindrop::Window::SurfaceProviders{
    class Metal : public SurfaceProvider{
        public:
            inline Metal(Window& window) : SurfaceProvider(window){}

            virtual SDL_MetalView createView() = 0;
            virtual void destroyView(SDL_MetalView view) = 0;
            virtual void* getLayer(SDL_MetalView view) = 0;

            static std::string_view API() {return "Metal";}
    };
}