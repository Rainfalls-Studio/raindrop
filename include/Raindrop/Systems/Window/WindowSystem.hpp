#pragma once

#include "Raindrop/Core/Systems/ISystem.hpp"
#include "Raindrop/Core/Scheduler/Subscription.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"
#include <memory>
#include <unordered_map>

namespace Raindrop::Window{
    class WindowSystem : public Systems::ISystem{
        public:
            virtual ~WindowSystem() = default;

            virtual std::expected<void, Error> initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual std::string name() const noexcept override;
            virtual std::vector<Systems::Dependency> dependencies() const noexcept override;

            void event();

            std::shared_ptr<Window> createWindow(const WindowConfig& config);
        
        private:
            Engine* _engine;
            std::unordered_map<SDL_WindowID, std::weak_ptr<Window>> _windows;

            Scheduler::Subscription eventSubscription;
            
    };
}