#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp"
#include "ImGuiModule.hpp"

namespace Raindrop::Render{
    struct ImGuiStorage{
        ImGuiService::Context* context;
    };

    class ImGuiStage : public Scheduler::IStage{
        public:
            ImGuiStage(const std::string& outputName);

            virtual ~ImGuiStage() override = default;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;

        
        private:
            Engine* _engine;
            std::weak_ptr<IRenderOutput> _output;
            std::weak_ptr<ImGuiModule> _imgui;
            Scheduler::Loop _loop;
            IRenderOutput::Name _outputName;

            void findOutput();

            Scheduler::HookResult preGUI();
            Scheduler::HookResult postGUI();
        
    };
}