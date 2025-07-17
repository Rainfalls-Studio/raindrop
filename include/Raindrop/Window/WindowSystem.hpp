#pragma once

#include "Raindrop/System/ISystem.hpp"
#include "Raindrop/Event/Manager.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"
#include <memory>
#include <unordered_map>

namespace Raindrop::Window{
    class WindowSystem : public System::ISystem{
        public:
            virtual ~WindowSystem() = default;

            virtual void initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual void event() override;

            virtual const char* name() const override;

            std::shared_ptr<Window> createWindow(const WindowConfig& config);
        
        private:
            Engine* _engine;
            std::unordered_map<SDL_WindowID, std::weak_ptr<Window>> _windows;
    };
}