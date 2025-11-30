#pragma once

#include <memory>
#include "IModule.hpp"

namespace Raindrop::Modules{
    class IModuleLoader{
        public:
            virtual ~IModuleLoader() = default;

            virtual std::shared_ptr<IModule> create() = 0;
    };
}