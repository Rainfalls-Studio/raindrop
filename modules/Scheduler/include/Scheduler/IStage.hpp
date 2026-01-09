#pragma once

#include "StageInitHelper.hpp"
#include "StageResult.hpp"

namespace Scheduler{
    class IStage {
        public:
            virtual ~IStage() = default;
            
            virtual const char* name() const = 0;

            inline virtual void initialize(StageInitHelper& helper [[maybe_unused]]) {}
            virtual void shutdown() {}
            virtual StageResult execute() = 0;
        };
}