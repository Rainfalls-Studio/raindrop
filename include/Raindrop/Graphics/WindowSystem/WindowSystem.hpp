#pragma once

#include <unordered_map>
#include <memory>
#include <spdlog/logger.h>
#include "Raindrop/Graphics/System/IRenderSystem.hpp"
#include "Raindrop/Window/WindowSystem.hpp"
#include "WindowContext.hpp"

namespace Raindrop::Graphics::WindowSystem{
    class WindowSystem : public System::IRenderSystem{
        public:
            virtual ~WindowSystem() = default;

            virtual void initialize(GraphicsEngine& engine) override;
            virtual void postInitialize() override;
            virtual void shutdown() override;

            virtual void preRender() override;

            virtual const char* name() const override;
            virtual std::vector<Dependency> dependencies() const override;
            
        private:
            GraphicsEngine* _engine;
            std::shared_ptr<spdlog::logger> _logger;
            std::unordered_map<Raindrop::Window::Window*, std::unique_ptr<WindowContext>> _contexts;
            std::weak_ptr<Raindrop::Window::WindowSystem> _windowSystem;

            void createLogger();
    };
}