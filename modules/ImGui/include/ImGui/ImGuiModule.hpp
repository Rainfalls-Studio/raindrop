#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include <Raindrop/Engine.hpp>
#include <RenderCore/RenderCoreModule.hpp>
#include "ImGuiContext.hpp"

namespace Raindrop::ImGui{
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

            std::shared_ptr<ImGuiContext> createContext(std::shared_ptr<Render::IRenderOutput> output);

        private:
            Engine* _engine;
            std::shared_ptr<Render::RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;

            void createLogger();
            void initializeImGui();
    };
}