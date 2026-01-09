#include "ImGui/ImGuiModule.hpp"

#include <Raindrop/Modules/InitHelper.hpp>
#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>

extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new ImGui::ImGuiModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}


namespace ImGui{
    ImGuiModule::ImGuiModule(){}
    ImGuiModule::~ImGuiModule(){}

    Raindrop::Result ImGuiModule::initialize(Raindrop::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<Render::RenderCoreModule>("RenderCore");

        createLogger();

        return Raindrop::Result::Success();
    }

    void ImGuiModule::createLogger(){
        _logger = spdlog::stdout_color_mt("ImGui");
    }

    void ImGuiModule::shutdown(){
    }

    std::shared_ptr<ImGuiContext> ImGuiModule::createContext(std::shared_ptr<Render::IRenderOutput> output){
        return std::make_shared<ImGuiContext>(
            _core,
            output
        );
    }
}