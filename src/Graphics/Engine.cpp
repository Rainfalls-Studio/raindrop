#include "Raindrop/Graphics/Engine.hpp"

namespace Raindrop::Graphics{
    EngineConfig::EngineConfig(Raindrop::Engine& engine) : _engine{engine}{}

    EngineConfig& EngineConfig::setWindow(std::shared_ptr<Window::Window> window){
        _window = std::move(window);
        return *this;
    }

    EngineConfig& EngineConfig::setAssetManager(std::shared_ptr<Asset::Manager> assetManager){
        _assetManager = std::move(assetManager);
        return *this;
    }

    Engine::Engine(EngineConfig& config) : _engine{config._engine}{}
    Engine::~Engine(){}
}