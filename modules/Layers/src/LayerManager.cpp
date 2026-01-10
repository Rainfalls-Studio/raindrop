#include "Layers/LayerManager.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Layers{
    LayerManager::LayerManager(std::shared_ptr<spdlog::logger> logger){
        setupContext(logger);
        createRoot();

        SPDLOG_LOGGER_INFO(_ctx.logger, "Layer manager initialized !");
    }

    void LayerManager::setupContext(std::shared_ptr<spdlog::logger> logger){
        _ctx.logger = logger;
    }

    void LayerManager::createRoot(){
        _root = std::shared_ptr<Layer>(new Layer("root", 0.f, nullptr, _ctx));
    }
}