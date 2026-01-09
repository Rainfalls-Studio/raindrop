#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include "WindowConfig.hpp"
#include "Window.hpp"
#include <memory>
#include <unordered_map>

namespace Raindrop{
    class Engine;
}

namespace Window{
    class WindowModule : public Raindrop::IModule{
        public:
            virtual ~WindowModule() = default;

            virtual Raindrop::Result initialize(Raindrop::InitHelper& init) override;
            virtual void shutdown() override;

            void event();

            std::shared_ptr<Window> createWindow(const WindowConfig& config);
        
        private:
            Raindrop::Engine* _engine;
            std::unordered_map<SDL_WindowID, std::weak_ptr<Window>> _windows;
            
    };
}