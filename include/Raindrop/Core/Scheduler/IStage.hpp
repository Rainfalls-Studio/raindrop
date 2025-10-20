#pragma once

#include "StageInitHelper.hpp"
#include "StageResult.hpp"

namespace Raindrop::Scheduler{
    class IStage {
        public:
            virtual ~IStage() = default;
            
            virtual const char* name() const = 0;

            virtual void initialize(StageInitHelper& helper) = 0;
            virtual void shutdown() {}
            virtual StageResult execute() = 0;
        };
}