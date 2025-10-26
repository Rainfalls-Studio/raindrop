#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "RenderGraphModule.hpp"
#include <RenderGraph/FrameGraph.hpp>

namespace Raindrop::Render{
    class RenderGraphRenderStage : public Scheduler::IStage{
        public:
            RenderGraphRenderStage(std::weak_ptr<RenderGraph> graph);
            virtual ~RenderGraphRenderStage() = default;

            virtual const char* name() const override;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;
        
        private:
            Scheduler::Loop _loop;
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::weak_ptr<RenderGraph> _graph;

            
            void invalidate();
    };
}