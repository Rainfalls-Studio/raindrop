#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "RenderCoreModule.hpp"

#include <RenderGraph/FrameGraphPrerequisites.hpp>

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

            crg::FrameGraph* getFrameGraph();

        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::unique_ptr<crg::GraphContext> _context;
            std::unique_ptr<crg::ResourceHandler> _resourceHandler;
            std::unique_ptr<crg::FrameGraph> _frameGraph;

            Modules::Result buildContext();
            void buildResourceHandler();
            void buildFrameGraph();

            void destroyFrameGraph();
            void destroyResourceHandler();
            void destroyContext();
    };
}