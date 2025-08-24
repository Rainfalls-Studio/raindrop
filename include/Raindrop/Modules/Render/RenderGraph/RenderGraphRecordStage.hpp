#pragma once

#include "Raindrop/Core/Layers/Layer.hpp"
#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Scheduler/Loop.hpp"
#include "RenderGraphModule.hpp"
#include <RenderGraph/FrameGraph.hpp>

namespace Raindrop::Render{
    class RenderGraphRecordStage : public Scheduler::IStage{
        public:
            RenderGraphRecordStage(Layers::Layer layer);

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
        
        private:
            Engine* _engine;
            Scheduler::Loop _loop;
            Layers::Layer _layer;
            bool _submited = false;

            Scheduler::HookResult render();
    };
}