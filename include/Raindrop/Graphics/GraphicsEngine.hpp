#pragma once

#include <memory>
#include "System/Manager.hpp"
#include "Raindrop/Engine.hpp"

namespace Raindrop::Graphics{
    class GraphicsEngine{
        public:
            GraphicsEngine() = default;
            ~GraphicsEngine() = default;

            void initialize(Engine& engine);
            void postInitialize();
            void preShutdown();
            void shutdown();

            void preRender();
            void render();
            void postRender();

            System::Manager& getSystemManager();
            Engine& getEngine();

        private:
            Engine* _engine;
            std::unique_ptr<System::Manager> _manager;
    };
}