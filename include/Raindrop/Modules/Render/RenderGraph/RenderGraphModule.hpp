#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "../Core/RenderCoreModule.hpp"

#include <RenderGraph/FrameGraphPrerequisites.hpp>
#include <RenderGraph/FrameGraph.hpp>

namespace Raindrop::Render{
    class RenderGraphModule : public Modules::IModule{
        public:
            RenderGraphModule();
            virtual ~RenderGraphModule() override;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;
 
            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            inline virtual Modules::Result dependencyReload(const Name& dep) override;
            inline virtual Modules::Result dependencyShutdown(const Name& dep) override;

            std::shared_ptr<crg::FrameGraph> createGraph(const std::string& name = "Render graph");
            crg::GraphContext& context();

        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;

            bool _pendingRecompile = true;

            std::unique_ptr<crg::GraphContext> _context;
            std::unique_ptr<crg::ResourceHandler> _resourceHandler;
            

            Modules::Result buildContext();
            void buildResourceHandler();

            void destroyResourceHandler();
            void destroyContext();
            void setupLogger();
    };
}