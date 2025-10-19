#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "../Core/RenderCoreModule.hpp"
#include "ImGuiService.hpp"

namespace Raindrop::Render{
    class ImGuiModule : public Modules::IModule{
        public:
            ImGuiModule();
            virtual ~ImGuiModule() override;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;
 
            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            inline virtual Modules::Result dependencyReload(const Name& dep) override;
            inline virtual Modules::Result dependencyShutdown(const Name& dep) override;

            ImGuiService::Context& begin(const IRenderOutput::Name& target, SharedRenderOutput output);
            void end(ImGuiService::Context& context);

        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;

            std::unique_ptr<ImGuiService> _service;

            void createLogger();
            Modules::Result createService();
            void shutdownService();

            void createDescriptor();
            void initializeImGui();
    };
}