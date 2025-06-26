#include "Raindrop/Graphics/Config.hpp"

namespace Raindrop::Graphics{
    Config::Config(Raindrop::Engine& engine) : _engine{engine}{}

    Config& Config::setAssetManager(std::shared_ptr<Asset::Manager> assetManager){
        _assetManager = assetManager;
        return *this;
    }

    Config& Config::setLayerManager(std::shared_ptr<Layer::Manager> layerManager){
        _layerManager = layerManager;
        return *this;
    }

    Config& Config::setWindowManager(std::shared_ptr<Window::Manager> windowManager){
        _windowManager = windowManager;
        return *this;
    }

    Config& Config::setHeadless(bool headless){
        _headless = headless;
        return *this;
    }
}