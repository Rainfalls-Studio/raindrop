#include "Raindrop/Modules/Render/ImGui/ImGuiStage.hpp"
#include "Raindrop/Modules/Render/ImGui/ImGuiModule.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"

namespace Raindrop::Render{
    ImGuiStage::ImGuiStage(const std::string& outputName) : _outputName(outputName){}

    void ImGuiStage::initialize(Scheduler::StageInitHelper& helper){
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

        helper.registerHook(Hook{
            Phase::PRE_GUI,
            "Prepare GUI onto : " + _outputName,
            [this]{ return preGUI();}
        });

        helper.registerHook(Hook{
            Phase::POST_GUI,
            "Render GUI onto : " + _outputName,
            [this]{ return postGUI(); }
        });
    }

    void ImGuiStage::shutdown(){
        
    }
    
    void ImGuiStage::findOutput(){
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

    Scheduler::HookResult ImGuiStage::preGUI(){
        using namespace Scheduler;
        auto imgui = _imgui.lock();

        if (!imgui){
            spdlog::error("The ImGui module is not valid");
            return HookResult::Skip("The ImGui module is not valid");
        }

        auto output = _output.lock();

        if (!output){
            findOutput();
            
            output = _output.lock();

            if (!output){
                return HookResult::Skip("No render output");
            }
        }

        auto& context = imgui->begin(_outputName, output);
        auto& storage = _loop.getOrEmplaceStorage<ImGuiStorage>();

        storage.context = &context;

        return HookResult::Continue();
    }

    Scheduler::HookResult ImGuiStage::postGUI(){
        using namespace Scheduler;
        auto imgui = _imgui.lock();

        if (!imgui){
            spdlog::error("The ImGui module is not valid");
            return HookResult::Skip("The ImGui module is not valid");
        }
        auto& storage = _loop.getOrEmplaceStorage<ImGuiStorage>();

        imgui->end(*storage.context);

        return HookResult::Continue();
    }
}