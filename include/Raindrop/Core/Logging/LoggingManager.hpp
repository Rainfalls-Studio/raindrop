#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace Raindrop::Core::Logging{
    class LoggingManager{
        public:
            static void Init();
            static void Shutdown();

            static std::shared_ptr<spdlog::logger> GetLogger(const std::string& name);

        private:
            static inline std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> _loggers;
    };
}