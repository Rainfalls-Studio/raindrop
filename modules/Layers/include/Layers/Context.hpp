#pragma once

#include <memory>
#include <shared_mutex>
#include <spdlog/spdlog.h>

namespace Layers{
    struct Context{
        std::shared_ptr<spdlog::logger> logger;
        std::shared_mutex mtx;
    };
}