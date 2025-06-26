#pragma once

#include "Raindrop/Asset/Manager.hpp"
#include "Raindrop/Engine.hpp"
#include "Raindrop/Layer/Manager.hpp"
#include "Raindrop/Window/Manager.hpp"

namespace Raindrop::Graphics{
    class Config{
        public:
            Config(Raindrop::Engine& engine);

            Config& setAssetManager(std::shared_ptr<Asset::Manager> assetManager);
            Config& setLayerManager(std::shared_ptr<Layer::Manager> layerManager);
            Config& setWindowManager(std::shared_ptr<Window::Manager> windowManager);
            Config& setHeadless(bool headless = true);

            Raindrop::Engine& _engine;
            std::shared_ptr<Asset::Manager> _assetManager;
            std::shared_ptr<Layer::Manager> _layerManager;
            std::shared_ptr<Window::Manager> _windowManager;
            bool _headless = false;
    };
}