#include "Raindrop/Modules/Render/ImGui/ImGuiEndStage.hpp"
#include "Raindrop/Modules/Render/ImGui/ImGuiModule.hpp"
#include "Raindrop/Modules/Render/ImGui/LoopStorage.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"

namespace Raindrop::Render{
    ImGuiEndStage::ImGuiEndStage(const std::string& outputName) : _outputName(outputName){}

    void ImGuiEndStage::initialize(Scheduler::StageInitHelper& helper){
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

    void ImGuiEndStage::shutdown(){
        
    }

    const char* ImGuiEndStage::name() const{
        return "ImGuiEnd";
    }

    
    void ImGuiEndStage::findOutput(){
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

    Scheduler::StageResult ImGuiEndStage::execute(){
        using namespace Scheduler;
        auto imgui = _imgui.lock();

        if (!imgui){
            spdlog::error("The ImGui module is not valid");
            return StageResult::Skip("The ImGui module is not valid");
        }
        auto& storage = _loop.getOrEmplaceStorage<ImGuiStorage>();

        imgui->end(*storage.context);

        return StageResult::Continue();
    }
}