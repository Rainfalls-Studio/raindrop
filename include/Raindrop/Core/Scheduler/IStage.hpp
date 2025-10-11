#pragma once

#include "StageInitHelper.hpp"

namespace Raindrop::Scheduler{
    class IStage {
        public:
            virtual ~IStage() = default;
            virtual void initialize(StageInitHelper& helper) = 0;
            virtual void shutdown() {}
    };
}