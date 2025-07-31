#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Core/Scheduler/Subscription.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"
#include <memory>
#include <unordered_map>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Window{
    class WindowModule : public Modules::IModule{
        public:
            virtual ~WindowModule() = default;

            virtual Modules::Result initialize(Modules::InitHelper& init) override;
            virtual void shutdown() override;

            virtual std::string name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            void event();

            std::shared_ptr<Window> createWindow(const WindowConfig& config);
        
        private:
            Engine* _engine;
            std::unordered_map<SDL_WindowID, std::weak_ptr<Window>> _windows;

            Scheduler::Subscription eventSubscription;
            
    };
}