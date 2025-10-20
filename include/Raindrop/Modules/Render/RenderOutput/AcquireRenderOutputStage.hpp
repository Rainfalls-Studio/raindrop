#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "IRenderOutput.hpp"

namespace Raindrop::Render{
    class AcquireRenderOutputStage : public Scheduler::IStage{
        public:
            AcquireRenderOutputStage(const std::string& outputName);

            virtual ~AcquireRenderOutputStage() override = default;

            virtual const char* name() const override;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;
        
        private:
            Engine* _engine;
            std::weak_ptr<IRenderOutput> _output;
            Scheduler::Loop _loop;
            IRenderOutput::Name _outputName;

            void findOutput();
    };
}