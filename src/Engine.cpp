#include "Raindrop/Engine.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop{
    Engine::Engine(){
        spdlog::set_level(spdlog::level::trace);
    }

    std::shared_ptr<Engine> Engine::Create(){
        return std::make_shared<Engine>();
    }
}