#pragma once

#include <imgui.h>

#include <Raindrop/Modules/Render/RenderOutput/IRenderOutput.hpp>
#include <Raindrop/Modules/Render/Core/RenderCoreModule.hpp>

namespace Raindrop::ImGui{
    class ImGuiContext{
        public:
            using NativeContext = ::ImGuiContext;

            ImGuiContext(std::shared_ptr<Render::RenderCoreModule> core, std::shared_ptr<Render::IRenderOutput> output);
            ~ImGuiContext();

            void begin();
            void end();
            void render(vk::CommandBuffer cmdBuffer);

        private:
            std::shared_ptr<Render::RenderCoreModule> _core;
            std::shared_ptr<Render::IRenderOutput> _output;
            NativeContext* _ctx;

            vk::DescriptorPool _pool;

            void setupBackend();
            vk::DescriptorPool createDescriptorPool();
            void destroyDescriptorPool();

    };


    class BeginStage : public Scheduler::IStage{
        public:
            BeginStage(std::shared_ptr<ImGuiContext> context);
            virtual ~BeginStage() = default;

            virtual const char* name() const override;

            // inline virtual void initialize(StageInitHelper& helper [[maybe_unused]]) {}
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;

        private:
            std::weak_ptr<ImGuiContext> _ctx;
    };


    class EndStage : public Scheduler::IStage{
        public:
            EndStage(std::shared_ptr<ImGuiContext> context);
            virtual ~EndStage() = default;

            virtual const char* name() const override;

            // inline virtual void initialize(StageInitHelper& helper [[maybe_unused]]) {}
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;

        private:
            std::weak_ptr<ImGuiContext> _ctx;
    };


    class RenderStage : public Scheduler::IStage{
        public:
            RenderStage(std::shared_ptr<ImGuiContext> context, std::shared_ptr<Render::RenderCommandContext> cmdCtx);
            virtual ~RenderStage() = default;

            virtual const char* name() const override;

            // inline virtual void initialize(StageInitHelper& helper [[maybe_unused]]) {}
            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;

        private:
            std::weak_ptr<ImGuiContext> _ctx;
            std::weak_ptr<Render::RenderCommandContext> _cmdCtx;
    };
}