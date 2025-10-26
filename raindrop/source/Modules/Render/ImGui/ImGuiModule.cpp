#include "Raindrop/Modules/Render/ImGui/ImGuiModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"
#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Raindrop/Modules/Render/ImGui/backend/imgui_impl_sdl3.h"

namespace Raindrop::Render{
    ImGuiModule::ImGuiModule(){}
    ImGuiModule::~ImGuiModule(){}

    Modules::Result ImGuiModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _core = helper.getDependencyAs<RenderCoreModule>("RenderCore");

        createLogger();

        {
            auto result = createService();
            if (result.level() != Modules::Result::Level::SUCCESS){
                return result;
            }
        }

        return Modules::Result::Success();
    }

    void ImGuiModule::createLogger(){
        _logger = spdlog::stdout_color_mt("ImGui");
    }

    void ImGuiModule::shutdown(){
        shutdownService();
    }

    Modules::Result ImGuiModule::createService(){
        try{
            _service = std::make_unique<ImGuiService>(_core, _logger);
        } catch (const std::exception &e){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create ImGui service : {}", e.what());
            return Modules::Result::Error(e.what());
        }

        return Modules::Result::Success();
    }

    void ImGuiModule::shutdownService(){
        _service.reset();
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
        if (dep == "RenderCore"){
            shutdownService();
            return createService();
        }

        return Modules::Result::Success();
    }

    Modules::Result ImGuiModule::dependencyShutdown(const Name& dep){
        if (dep == "RenderCore"){
            shutdownService();
        }

        return Modules::Result::Success();
    }


    ImGuiService::Context& ImGuiModule::begin(const IRenderOutput::Name& name, SharedRenderOutput output){
        auto& context = _service->getOrCreateContext(name, output);

        _service->begin(context);

        return context;
    }

    void ImGuiModule::end(ImGuiService::Context& context){
        _service->end(context);
    }

    ImGuiService& ImGuiModule::service(){
        return *_service;
    }

}