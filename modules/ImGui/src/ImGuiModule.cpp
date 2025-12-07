#include "ImGui/ImGuiModule.hpp"

#include <Raindrop/Modules/InitHelper.hpp>
#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::ImGui::ImGuiModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
}


namespace Raindrop::ImGui{
    ImGuiModule::ImGuiModule(){}
    ImGuiModule::~ImGuiModule(){}

    Modules::Result ImGuiModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<Render::RenderCoreModule>("RenderCore");

        createLogger();

        return Modules::Result::Success();
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

    Modules::Name ImGuiModule::name() const noexcept{
        return RAINDROP_CURRENT_MODULE_NAME;
    }

    Modules::DependencyList ImGuiModule::dependencies() const noexcept{
        return {
            Modules::Dependency("RenderCore")
        };
    }

    Modules::Result ImGuiModule::dependencyReload(const Modules::Name& dep [[__maybe_unused__]]){

        return Modules::Result::Success();
    }

    Modules::Result ImGuiModule::dependencyShutdown(const Modules::Name& dep [[__maybe_unused__]]){

        return Modules::Result::Success();
    }
}