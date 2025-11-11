#pragma once

#include <Raindrop/Raindrop.hpp>

namespace Planet{
    class RenderBehavior : public Raindrop::Scene::IBehavior{
        public:
            RenderBehavior(std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx);

            virtual void initialize(Raindrop::Engine& engine, Raindrop::Scene::Scene& scene) override;
            virtual void shutdown() override;

            virtual void execute() override;

        private:
            Raindrop::Scene::Scene* _scene;
            Raindrop::Scene::BehaviorID _snapshotService;
            Raindrop::Scene::BehaviorID _planetService;
            Raindrop::Behaviors::FrameSnapshotService::SlotID _planetSlot;
            Raindrop::Behaviors::FrameSnapshotService::SlotID _cameraSlot;


            std::shared_ptr<Raindrop::Render::RenderCoreModule> _core;
            std::shared_ptr<Raindrop::Asset::AssetModule> _assets;


            vk::Pipeline _pipeline;
            vk::PipelineLayout _pipelineLayout;
            std::shared_ptr<Raindrop::Render::Shader> _vertexShader;
            std::shared_ptr<Raindrop::Render::Shader> _fragmentShader;

            std::weak_ptr<Raindrop::Render::IRenderOutput> _output;
            std::weak_ptr<Raindrop::Render::RenderCommandContext> _cmdCtx;

            void createPipelineLayout();
            void createPipeline();
        
    };
}