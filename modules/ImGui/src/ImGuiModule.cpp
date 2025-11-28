#include "Raindrop/Modules/ImGui/ImGuiModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"
#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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


    ImGuiModule::Name ImGuiModule::name() const noexcept{
        return "ImGui";
    }

    Modules::DependencyList ImGuiModule::dependencies() const noexcept{
        using Dependency = Modules::Dependency;
        return {
            Dependency("RenderCore")
        };
    }

    Modules::Result ImGuiModule::dependencyReload(const Name& dep){

        return Modules::Result::Success();
    }

    Modules::Result ImGuiModule::dependencyShutdown(const Name& dep){

        return Modules::Result::Success();
    }
}