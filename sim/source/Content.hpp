#pragma once

#include <Raindrop/Raindrop.hpp>

class ContentStage : public Raindrop::Scheduler::IStage{
    public:
        ContentStage(){}

        virtual ~ContentStage() override = default;

        virtual const char* name() const override{
            return "Content";
        }

        virtual void initialize(Raindrop::Scheduler::StageInitHelper&) override{}

        virtual void shutdown() override{
            
        }

        virtual Raindrop::Scheduler::StageResult execute() override{
            ImGui::Begin("Content");

            // ImGui::Image()

            ImGui::End();
            return Raindrop::Scheduler::StageResult::Continue();
        }
};