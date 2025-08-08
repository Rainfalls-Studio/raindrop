#pragma once

#include <vector>
#include "Hook.hpp"

namespace Raindrop::Scheduler{
    class IStage {
        public:
            virtual ~IStage() = default;
            virtual void registerHooks(std::vector<Hook>& hooks) = 0;
    };
}