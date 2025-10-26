#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "../Core/RenderCoreModule.hpp"
#include "RenderGraph.hpp"

#include <RenderGraph/FrameGraphPrerequisites.hpp>
#include <RenderGraph/FrameGraph.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Render{
    class RenderGraphModule : public Modules::IModule, public std::enable_shared_from_this<RenderGraphModule>{
        public:
            RenderGraphModule();
            virtual ~RenderGraphModule() override;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;
 
            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            inline virtual Modules::Result dependencyReload(const Name& dep) override;
            inline virtual Modules::Result dependencyShutdown(const Name& dep) override;

            std::shared_ptr<RenderGraph> createGraph(const std::string& name = "Render graph");
            crg::GraphContext& context();
            crg::ResourceHandler& resourceHandler();
            std::shared_ptr<RenderCoreModule> core();


        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::shared_ptr<spdlog::logger> _logger;

            std::unique_ptr<crg::GraphContext> _context;
            std::unique_ptr<crg::ResourceHandler> _resourceHandler;

            Modules::Result buildContext();
            void buildResourceHandler();

            void destroyResourceHandler();
            void destroyContext();
            void setupLogger();
    };
}