#include "Raindrop/Modules/Layers/LayerManager.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Layers{
    LayerManager::LayerManager(){
        setupContext();
        createRoot();

        SPDLOG_LOGGER_INFO(_ctx.logger, "Layer manager initialized !");
    }

    void LayerManager::setupContext(){
        _ctx.logger = spdlog::stdout_color_mt("Layers");
    }

    void LayerManager::createRoot(){
        _root = std::shared_ptr<Layer>(new Layer("root", 0.f, nullptr, _ctx));
    }
}