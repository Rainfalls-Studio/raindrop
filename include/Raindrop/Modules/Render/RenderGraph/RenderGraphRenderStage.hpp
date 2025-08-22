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
        
        private:
            Scheduler::Loop _loop;
            Engine* _engine;
            std::weak_ptr<RenderGraphModule> _weakRenderGraphModule = {};
            std::vector<crg::RunnableGraphPtr> _runnableGraphs;

            Scheduler::HookResult render();
    };
}