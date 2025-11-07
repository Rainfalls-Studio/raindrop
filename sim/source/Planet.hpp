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
        enum Face{
            X,
            Y,
            Z
        };

        struct ChunkNode{
            int8_t face; // X, -X, Y, -Y, Z, -Y
            uint8_t lod;
            glm::vec2 uvMin;
            glm::vec2 uvMax;
            float angularSize;
            ChunkNode* children[4];
            ChunkNode* parent;
        };

        std::array<ChunkNode, 6> _faces;

        struct PlanetGPU{
            vk::Buffer _vertexBuffer;
            VmaAllocation _vertexAllocation;

            vk::Buffer _indexBuffer;
            VmaAllocation _indexAllocation;

            vk::Buffer _instanceBuffer;
            VmaAllocation _instanceAllocation;

            uint32_t _vertexCount;
            uint32_t _indexCount;
            uint32_t _instanceCount;
            uint32_t _maxInstanceCount;
        };

        PlanetGPU _gpu;

        std::shared_ptr<Raindrop::Render::RenderCoreModule> _core;
        std::shared_ptr<Raindrop::Asset::AssetModule> _assets;


        vk::Pipeline _pipeline;
        vk::PipelineLayout _pipelineLayout;
        std::shared_ptr<Raindrop::Render::Shader> _vertexShader;
        std::shared_ptr<Raindrop::Render::Shader> _fragmentShader;

        std::shared_ptr<Raindrop::Render::IRenderOutput> _output;

        glm::quat _rotation = glm::quat(0.f, {1.f, 0.f, 0.f});

        void createPipelineLayout();
        void createPipeline();
        void createGridMesh(uint32_t xcells, uint32_t ycells);
        void createInstanceBuffer();
};