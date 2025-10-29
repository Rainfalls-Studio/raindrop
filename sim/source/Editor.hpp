#pragma once

#include <Raindrop/Raindrop.hpp>

class Editor : public Raindrop::Scheduler::IStage{
    public:
        Editor(){}

        virtual ~Editor() override = default;

        virtual const char* name() const override{
            return "Editor";
        }

        virtual void initialize(Raindrop::Scheduler::StageInitHelper&) override{}

        virtual void shutdown() override{
            
        }

        virtual Raindrop::Scheduler::StageResult execute() override{
            constexpr ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoBackground;

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

            ImGui::Begin("DockspaceHost", nullptr, window_flags);

            ImGui::PopStyleVar(2);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            ImGui::End();

            ImGui::Begin("Scene");
            ImGui::Text("Scene view content");
            ImGui::End();

            return Raindrop::Scheduler::StageResult::Continue();
        }
};