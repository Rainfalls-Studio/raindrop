#pragma once

namespace Raindrop::Window{
    class Window;
    class SurfaceProvider{
        public:
            inline SurfaceProvider(Window& window) : _window{window} {}
            virtual ~SurfaceProvider() = default;
            
        protected:
            Window& _window;
    };
}