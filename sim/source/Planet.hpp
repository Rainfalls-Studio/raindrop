#pragma once

#include <Raindrop/Raindrop.hpp>

class Planet{
    public:
        class RenderStage : public Raindrop::Scheduler::IStage{
            public:
                RenderStage(std::weak_ptr<Planet> planet, std::weak_ptr<Raindrop::Render::IRenderOutput> output, std::weak_ptr<Raindrop::Render::RenderCommandContext> cmdCtx);
                virtual ~RenderStage() override = default;

                virtual const char* name() const override;

                virtual void initialize(Raindrop::Scheduler::StageInitHelper& helper) override;
                virtual void shutdown() override;
                virtual Raindrop::Scheduler::StageResult execute() override;

            private:
                std::weak_ptr<Planet> _planet;
                std::weak_ptr<Raindrop::Render::IRenderOutput> _output;
                std::weak_ptr<Raindrop::Render::RenderCommandContext> _cmdCtx;
        };

        friend class RenderStage;

        Planet(Raindrop::Engine& engine, std::shared_ptr<Raindrop::Render::IRenderOutput> output);
        ~Planet();

    private:
        std::shared_ptr<Raindrop::Render::RenderCoreModule> _core;
        std::shared_ptr<Raindrop::Asset::AssetModule> _assets;

        vk::Buffer _vertexBuffer;
        vk::Buffer _indexBuffer;

        vk::Pipeline _pipeline;
        vk::PipelineLayout _pipelineLayout;
        std::shared_ptr<Raindrop::Render::Shader> _vertexShader;
        std::shared_ptr<Raindrop::Render::Shader> _fragmentShader;

        std::shared_ptr<Raindrop::Render::IRenderOutput> _output;

        glm::quat _rotation = glm::quat(0.f, {1.f, 0.f, 0.f});
        float _scale = 1.f;

        void createPipelineLayout();
        void createPipeline();
};