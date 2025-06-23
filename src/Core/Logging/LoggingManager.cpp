#include "Raindrop/Core/Logging/LoggingManager.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Core::Logging{
    void LoggingManager::Init() {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sink->set_pattern("[%T] [%^%n%$] [%l] %v");

        // Create loggers for each module
        _loggers["Audio"]   = std::make_shared<spdlog::logger>("Audio", sink);
        _loggers["Config"] = std::make_shared<spdlog::logger>("Config", sink);
        _loggers["Core"]  = std::make_shared<spdlog::logger>("Core", sink);
        _loggers["ECS"]  = std::make_shared<spdlog::logger>("ECS", sink);
        _loggers["Event"]  = std::make_shared<spdlog::logger>("Event", sink);
        _loggers["Graphics"]  = std::make_shared<spdlog::logger>("Graphics", sink);
        _loggers["Input"]  = std::make_shared<spdlog::logger>("Input", sink);
        _loggers["Layer"]  = std::make_shared<spdlog::logger>("Layer", sink);
        _loggers["Network"]  = std::make_shared<spdlog::logger>("Network", sink);
        _loggers["Physics"]  = std::make_shared<spdlog::logger>("Physics", sink);
        _loggers["Scene"]  = std::make_shared<spdlog::logger>("Scene", sink);
        _loggers["Window"]  = std::make_shared<spdlog::logger>("Window", sink);
        
        for (auto& [_, logger] : _loggers) {
            logger->set_level(spdlog::level::trace); // Global minimum level
            logger->flush_on(spdlog::level::err);    // Flush immediately on error
        }
    }

    std::shared_ptr<spdlog::logger> LoggingManager::GetLogger(const std::string& name) {
        return _loggers.at(name);
    }
}