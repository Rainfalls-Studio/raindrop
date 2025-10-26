#include "Raindrop/Modules/Render/ImGui/ImGuiBeginStage.hpp"
#include "Raindrop/Modules/Render/ImGui/ImGuiModule.hpp"
#include "Raindrop/Modules/Render/ImGui/LoopStorage.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"

namespace Raindrop::Render{
    ImGuiBeginStage::ImGuiBeginStage(const std::string& outputName) : _outputName(outputName){}

    void ImGuiBeginStage::initialize(Scheduler::StageInitHelper& helper){
        using namespace Scheduler;

        _loop = helper.loop();
        _engine = &helper.engine();

        auto imgui = helper.modules().getModuleAs<ImGuiModule>("ImGui");
        if (!imgui){
            spdlog::error("Can't find module \"ImGui\" !");
            return;
        }

        _imgui = imgui;

        findOutput();
    }

    void ImGuiBeginStage::shutdown(){
        
    }
    
    void ImGuiBeginStage::findOutput(){
        auto outputs = _engine->getModuleManager().getModuleAs<RenderOutputModule>("RenderOutput");
        if (!outputs){
            spdlog::error("Cannot get output \"{}\", There is no \"RenderOutput\" module registred", _outputName);
            return;
        }

        _output = outputs->getOutput(_outputName);

        if (_output.expired()){
            spdlog::error("Output \"{}\" not found", _outputName);
        }
    }

    const char* ImGuiBeginStage::name() const{
        return "ImGuiBegin";
    }

    Scheduler::StageResult ImGuiBeginStage::execute(){
        using namespace Scheduler;
        auto imgui = _imgui.lock();

        if (!imgui){
            spdlog::error("The ImGui module is not valid");
            return StageResult::Skip("The ImGui module is not valid");
        }

        auto output = _output.lock();

        if (!output){
            findOutput();
            
            output = _output.lock();

            if (!output){
                return StageResult::Skip("No render output");
            }
        }

        auto& context = imgui->begin(_outputName, output);
        float scale = output->scale();

        ImGuiStyle& style = ImGui::GetStyle();
        ImGuiStyle styleold = style; // Backup colors
        style = ImGuiStyle(); // IMPORTANT: ScaleAllSizes will change the original size, so we should reset all style config
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize  = 1.0f;
        style.PopupBorderSize  = 1.0f;
        style.FrameBorderSize  = 1.0f;
        style.TabBorderSize    = 1.0f;
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;
        style.ScaleAllSizes(scale);
        std::memcpy(style.Colors, styleold.Colors, sizeof(style.Colors));

        ImGui::GetIO().FontGlobalScale = scale;

        ImGui::ShowDemoWindow();

        auto& storage = _loop.getOrEmplaceStorage<ImGuiStorage>();

        storage.context = &context;

        return StageResult::Continue();
    }
}