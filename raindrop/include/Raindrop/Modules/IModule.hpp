#pragma once

#include <memory>
#include <unordered_map>

#include "Result.hpp"
#include "InitHelper.hpp"

namespace Raindrop{
    class InitHelper;
    using Name = std::string;  

    class IModule{
        public:

            virtual ~IModule() = default;

            virtual Result initialize(InitHelper& helper) = 0;
            inline virtual void postInitialize() {}

            inline virtual void preShutdown() {}
            inline virtual void shutdown() {}
    };
}