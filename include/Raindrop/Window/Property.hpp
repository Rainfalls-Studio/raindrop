#pragma once

#include <memory>

namespace Raindrop::Window{
    class Window;
    class Property{
        public:
            Property(Window& _window) : window{_window}{}
            virtual ~Property() = default;
        
            Window& window;
    };
}