#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Scene.hpp"

namespace Raindrop::Scene{
    class PhaseExecutionStage : public Scheduler::IStage{
        public:
            PhaseExecutionStage(std::shared_ptr<Scene> scene, PhaseID phase);

            virtual const char* name() const override;

            virtual Scheduler::StageResult execute() override;

        private:
            std::weak_ptr<Scene> _scene;
            PhaseID _phase;
    };
}