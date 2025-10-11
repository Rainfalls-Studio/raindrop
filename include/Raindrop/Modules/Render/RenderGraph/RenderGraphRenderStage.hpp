#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "RenderGraphModule.hpp"
#include <RenderGraph/FrameGraph.hpp>

namespace Raindrop::Render{
    class RenderGraphRenderStage : public Scheduler::IStage{
        public:
            RenderGraphRenderStage() = default;
            virtual ~RenderGraphRenderStage() = default;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
        
        private:
            Scheduler::Loop _loop;
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;
            std::weak_ptr<RenderGraphModule> _weakRenderGraphModule = {};

            struct CompiledGraph{
                crg::RunnableGraphPtr graph = {};
                bool invalid = true;
            };

            std::vector<CompiledGraph> _runnableGraphs;

            Scheduler::HookResult setupFrameGraph();
            Scheduler::HookResult render();
            void invalidate();
    };
}