#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Core/Scheduler/Subscription.hpp"
#include "RenderCoreModule.hpp"
#include <functional>
#include <vulkan/vulkan.hpp>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Render{
    class RenderSchedulerModule : public Modules::IModule{
        public:
            RenderSchedulerModule();
            virtual ~RenderSchedulerModule() = default;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;

            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            virtual Modules::Result dependencyReload(const Name& name) override;
            virtual Modules::Result dependencyShutdown(const Name& name) override;

            struct PreRenderResult{
                vk::Semaphore wait;
                vk::PipelineStageFlags waitStageFlags;
            };

            struct RenderResult{
                vk::Semaphore signal;
                vk::Fence fence;
            };

            using PrapareRenderCallback = std::function<void()>;
            using PreRenderCallback = std::function<PreRenderResult()>;
            using RenderCallback = std::function<RenderResult(const PreRenderResult&)>;
            using PostRenderCallback = std::function<void(const RenderResult&)>;


            inline void setPreparRenderCallback(const PrapareRenderCallback& callback){_preparRender = callback;}
            inline void setPreRenderCallback(const PreRenderCallback& callback){_preRender = callback;}
            inline void setRenderCallback(const RenderCallback& callback){_render = callback;}
            inline void setPostRender(const PostRenderCallback& callback){_postRender = callback;}

        private:
            Engine* _engine;
            std::shared_ptr<RenderCoreModule> _core;

            PreRenderResult _preRenderResult;
            RenderResult _renderResult;

            PrapareRenderCallback _preparRender;
            PreRenderCallback _preRender;
            RenderCallback _render;
            PostRenderCallback _postRender;

            Scheduler::Subscription _preRenderSubscription;
            Scheduler::Subscription _renderSubscription;
            Scheduler::Subscription _postRenderSubscription;

            void preRender();
            void render();
            void postRender();
    };
}