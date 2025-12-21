#pragma once

#include "Modules/Manager.hpp"

#include <filesystem>

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void initialize();

            Modules::Manager& getModuleManager() noexcept;

            std::filesystem::path executableDirectory();

        private:
            Modules::Manager _modules;
    };
}