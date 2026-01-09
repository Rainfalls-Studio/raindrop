#pragma once

#include "Modules/Manager.hpp"

#include <filesystem>

namespace Raindrop{
    class Engine{
        public:
            Engine();

            void initialize();

            ModuleManager& getModuleManager() noexcept;

            std::filesystem::path executableDirectory();

        private:
            ModuleManager _modules;
    };
}