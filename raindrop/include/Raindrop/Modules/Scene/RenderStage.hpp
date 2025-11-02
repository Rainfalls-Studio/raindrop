#pragma once

#include "Raindrop/Core/Scheduler/IStage.hpp"
#include "Raindrop/Core/Layers/Layer.hpp"
#include "Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp"
#include "Raindrop/Modules/Render/Core/RenderCommandContext.hpp"

namespace Raindrop::Scene{
    class RenderStage : public Scheduler::IStage{
        public:
            RenderStage(Layers::Layer layer, std::shared_ptr<Render::IRenderOutput> output, std::shared_ptr<Render::RenderCommandContext> cmdCtx);
            virtual ~RenderStage() override = default;

            virtual const char* name() const override;

            virtual void initialize(Scheduler::StageInitHelper& helper) override;
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;

        private:
            Layers::Layer _layer;
            std::weak_ptr<Render::IRenderOutput> _output;
            std::weak_ptr<Render::RenderCommandContext> _cmdCtx;
    };
}