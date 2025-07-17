#pragma once

#include "Raindrop/System/ISystem.hpp"
#include "Window.hpp"
#include <memory>

namespace Raindrop::Window{
    class WindowSystem : public System::ISystem{
        public:
            virtual ~WindowSystem() = default;

            virtual void initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual void event() override;

            virtual const char* name() const override;

            Window& getWindow();
            std::unique_ptr<Window> createEmptyWindow();
        
        private:
            Engine* _engine;
            std::unique_ptr<Window> _window;
    };
}