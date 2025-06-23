#pragma once

#include <memory>
#include <spdlog/logger.h>
#include "Raindrop/config.hpp"

namespace Raindrop::Window{
    class Context{
        public:
            Context();
            RDP_MOCK_VIRTUAL ~Context();

            RDP_MOCK_VIRTUAL std::shared_ptr<spdlog::logger> getLogger() const;

        private:
            std::shared_ptr<spdlog::logger> _logger;
    };
}