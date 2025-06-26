#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    DeviceConfig::DeviceConfig(Engine& gfxEngine) : _gfxEngine{gfxEngine}{}

    DeviceConfig& DeviceConfig::setAssetManager(std::shared_ptr<Asset::Manager> assetManager){
        _assetManager = assetManager;
        return *this;
    }

    DeviceConfig& DeviceConfig::setWindowManager(std::shared_ptr<Window::Manager> windowManager){
        _windowManager = windowManager;
        return *this;
    }

}