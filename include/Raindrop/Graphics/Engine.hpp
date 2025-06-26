#pragma once

#include "Raindrop/Asset/Manager.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Graphics/Config.hpp"
#include "Raindrop/Layer/Layer.hpp"
#include "Raindrop/Layer/Manager.hpp"
#include "Raindrop/Window/Manager.hpp"
#include "Raindrop/Window/Window.hpp"
#include "Backend/Device.hpp"

namespace Raindrop::Graphics{
    struct Context;

    class Engine{
        public:
            Engine(const Config& config);
            ~Engine();

            Raindrop::Engine& getEngine();

        private:
            Raindrop::Engine& _engine;
            std::shared_ptr<Asset::Manager> _assetManager;
            std::shared_ptr<Window::Manager> _windows;
            std::shared_ptr<Layer::Manager> _layers;

            std::shared_ptr<Backend::Device> _device;

            void createDevice();
    };
}