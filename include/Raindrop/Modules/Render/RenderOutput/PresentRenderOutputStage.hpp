#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "IRenderOutput.hpp"

namespace Raindrop::Render{
    class PresentRenderOutputStage : public Scheduler::IStage{
        public:
            PresentRenderOutputStage(const std::string& outputName);

            virtual ~PresentRenderOutputStage() override = default;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
        
        private:
            Engine* _engine;
            std::weak_ptr<IRenderOutput> _output;
            Scheduler::Loop _loop;
            IRenderOutput::Name _outputName;

            void findOutput();

            Scheduler::HookResult preRender();
            Scheduler::HookResult postRender();
        
    };
}