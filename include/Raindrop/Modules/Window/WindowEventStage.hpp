#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"

namespace Raindrop::Window{
    class EventStage : public Scheduler::IStage{
        public:
            virtual ~EventStage() = default;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
    };
}