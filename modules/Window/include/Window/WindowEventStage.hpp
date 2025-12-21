#pragma once

#include <Scheduler/IStage.hpp>
#include "WindowModule.hpp"

namespace Raindrop::Window{
    class EventStage : public Scheduler::IStage{
        public:
            virtual ~EventStage() = default;

            virtual const char* name() const override;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual Scheduler::StageResult execute() override;
        
        private:
            std::weak_ptr<WindowModule> _windowModule;
    };
}