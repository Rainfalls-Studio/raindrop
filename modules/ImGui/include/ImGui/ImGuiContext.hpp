#pragma once

#include <Event/Event.hpp>
#include <Event/MouseEvent.hpp>
#include <Event/KeyboardEvent.hpp>
#include <Window/WindowEvents.hpp>
#include <imgui.h>

#include <Layers/Layer.hpp>
#include <RenderOutput/IRenderOutput.hpp>
#include <RenderCore/RenderCoreModule.hpp>

namespace ImGui{
    class ImGuiContext{
        public:
            using NativeContext = ::ImGuiContext;

            ImGuiContext(std::shared_ptr<Render::RenderCoreModule> core, std::shared_ptr<Render::IRenderOutput> output);
            ~ImGuiContext();

            void begin();
            void end();
            void render(vk::CommandBuffer cmdBuffer);

            void setLayer(const std::shared_ptr<Layers::Layer>& layer);
            std::shared_ptr<Layers::Layer> layer();

        private:
            std::shared_ptr<Render::RenderCoreModule> _core;
            std::shared_ptr<Render::IRenderOutput> _output;
            std::shared_ptr<Layers::Layer> _layer;

            std::shared_ptr<Layers::Subscriber<Event::Event>> _eventSubscriber;

            std::mutex _mtx;

            struct EventBuffer{
                using Fnc = std::function<void(::ImGuiIO&)>;
                std::vector<Fnc> events;

                bool wantCaptureMouse;
                bool wantCaptureKeyboard;
            };

            EventBuffer _frontEventCaptures;
            EventBuffer _backEventCaptures;

            NativeContext* _ctx;

            vk::DescriptorPool _pool;

            void setupBackend();
            vk::DescriptorPool createDescriptorPool();
            void destroyDescriptorPool();

            bool handleEvent(const Event::Event& e);
            bool handleMouseEvent(const Event::MouseEvent& e);
            bool handleKeyboardEvent(const Event::KeyboardEvent& e);
            bool handleWindowEvent(const Window::Events::WindowEvent& e);

    };

    class EventStage : public Scheduler::IStage{
        public:
            EventStage(std::shared_ptr<ImGuiContext> context);
            virtual ~EventStage() = default;

            virtual const char* name() const override;

            virtual void shutdown() override;
            virtual Scheduler::StageResult execute() override;
            
        private:
            std::weak_ptr<ImGuiContext> _ctx;
    };

    class BeginStage : public Scheduler::IStage{
        public:
            BeginStage(std::shared_ptr<ImGuiContext> context);
            virtual ~BeginStage() = default;

            virtual const char* name() const override;

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