#include "Raindrop/Graphics/Engine.hpp"
#include "Raindrop/Graphics/Backend/Device.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Device.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"
#include "Raindrop/Graphics/Config.hpp"

namespace Raindrop::Graphics{
    Engine::Engine(const Config& config) : 
        _engine{config._engine},
        _assetManager{config._assetManager},
        _windows{config._windowManager},
        _layers{config._layerManager}
    {
        createDevice();
    }

    Engine::~Engine(){}

    void Engine::createDevice(){

        Backend::Vulkan::DeviceConfig config(*this);

        config
            .setAssetManager(_assetManager)
            .setWindowManager(_windows);

        _device = std::make_shared<Backend::Vulkan::Device>(config);
    }

    Raindrop::Engine& Engine::getEngine(){
        return _engine;
    }

}