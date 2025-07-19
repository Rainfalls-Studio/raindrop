#pragma once

#include <list>
#include <memory>
#include <spdlog/logger.h>
#include "Raindrop/Graphics/System/IRenderSystem.hpp"
#include "Raindrop/Window/Window.hpp"
#include "../Core/CoreSystem.hpp"
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

            void registerWindow(std::shared_ptr<Window::Window> window);
            
        private:
            GraphicsEngine* _engine;
            std::shared_ptr<spdlog::logger> _logger;

            std::shared_ptr<WindowContext> _window;

            void createLogger();
            void createWorker();
            void buildContext(WindowContext& ctx);
            void destroyContext(WindowContext& ctx);
    };
}