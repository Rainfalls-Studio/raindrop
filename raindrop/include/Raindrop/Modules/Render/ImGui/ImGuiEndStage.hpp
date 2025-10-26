#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp"
#include "ImGuiModule.hpp"

namespace Raindrop::Render{
    class ImGuiEndStage : public Scheduler::IStage{
        public:
            ImGuiEndStage(const std::string& outputName);

            virtual ~ImGuiEndStage() override = default;

            const char* name() const override;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;

        
        private:
            Engine* _engine;
            std::weak_ptr<IRenderOutput> _output;
            std::weak_ptr<ImGuiModule> _imgui;
            Scheduler::Loop _loop;
            IRenderOutput::Name _outputName;

            void findOutput();
        
    };
}