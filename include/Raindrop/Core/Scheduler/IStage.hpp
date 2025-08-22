#pragma once

#include "StageInitHelper.hpp"
#include "Hook.hpp"

namespace Raindrop::Scheduler{
    class IStage {
        public:
            virtual ~IStage() = default;
            virtual void initialize(StageInitHelper& helper) = 0;
    };
}