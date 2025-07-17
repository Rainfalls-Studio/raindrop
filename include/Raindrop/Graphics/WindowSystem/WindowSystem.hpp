#pragma once

#include <unordered_map>
#include <memory>
#include "Raindrop/Graphics/System/IRenderSystem.hpp"
#include "Raindrop/Graphics/RenderSystem.hpp"
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
            
        private:
            Engine* _engine;
            std::unordered_map<Raindrop::Window::Window*, std::unique_ptr<WindowContext>> _contexts;
            std::weak_ptr<Raindrop::Window::WindowSystem> _windowSystem;
    };
}