#pragma once

#include "../../Engine.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    class DeviceConfig{
        public:
            DeviceConfig(Engine& gfxEngine);

            DeviceConfig& setAssetManager(std::shared_ptr<Asset::Manager> assetManager);
            DeviceConfig& setWindowManager(std::shared_ptr<Window::Manager> windowManager);

            Engine& _gfxEngine;
            std::shared_ptr<Asset::Manager> _assetManager;
            std::shared_ptr<Window::Manager> _windowManager;
    };
}