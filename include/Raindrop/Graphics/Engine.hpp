#pragma once

#include "../Asset/Manager.hpp"
#include "../Window/Window.hpp"

namespace Raindrop::Graphics{
    class Context;
    class EngineConfig{
        friend class Engine;
        public:
            EngineConfig(Raindrop::Engine& engine);

            EngineConfig& setWindow(std::shared_ptr<Window::Window> window);
            EngineConfig& setAssetManager(std::shared_ptr<Asset::Manager> assetManager);

        private:
            Raindrop::Engine& _engine;
            std::shared_ptr<Window::Window> _window;
            std::shared_ptr<Asset::Manager> _assetManager;
    };

    class Engine{
        public:
            Engine(EngineConfig& config);
            ~Engine();

        private:
            Raindrop::Engine& _engine;
            
    };
}