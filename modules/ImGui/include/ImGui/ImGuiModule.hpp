#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include <Raindrop/Engine.hpp>
#include <RenderCore/RenderCoreModule.hpp>
#include "ImGuiContext.hpp"

namespace Raindrop::ImGui{
    class ImGuiModule : public IModule{
        public:
            ImGuiModule();
            virtual ~ImGuiModule() override;

            virtual Result initialize(InitHelper& helper) override;
            virtual void shutdown() override;

            std::shared_ptr<ImGuiContext> createContext(std::shared_ptr<Render::IRenderOutput> output);

        private:
            Engine* _engine;
            std::shared_ptr<Render::RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;

            void createLogger();
            void initializeImGui();
    };
}