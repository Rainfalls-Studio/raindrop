#pragma once

#include <memory>
#include <unordered_map>
#include <Scheduler/IStage.hpp>
#include <RenderCore/RenderCommandContext.hpp>

namespace Raindrop::Render{
    class IRenderOutput{
        public:
            using Name = std::string;

            class BeginStage : public Scheduler::IStage{
                public:
                    BeginStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx);

                    virtual const char* name() const override;
                    virtual Scheduler::StageResult execute() override;

                private:
                    std::weak_ptr<IRenderOutput> _output;
                    std::weak_ptr<RenderCommandContext> _cmdCtx;
            };

            class EndStage : public Scheduler::IStage{
                public:
                    EndStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx);

                    virtual const char* name() const override;
                    virtual Scheduler::StageResult execute() override;

                private:
                    std::weak_ptr<IRenderOutput> _output;
                    std::weak_ptr<RenderCommandContext> _cmdCtx;
            };

            class PresentStage : public Scheduler::IStage{
                public:
                    PresentStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx);

                    virtual ~PresentStage() override = default;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual void shutdown() override;
                    virtual Scheduler::StageResult execute() override;
                
                private:
                    Engine* _engine;
                    std::weak_ptr<IRenderOutput> _output;
                    std::weak_ptr<RenderCommandContext> _cmdCtx;
            };

            class AcquireStage : public Scheduler::IStage{
                public:
                    AcquireStage(std::shared_ptr<IRenderOutput> output, std::shared_ptr<RenderCommandContext> cmdCtx);

                    virtual ~AcquireStage() override = default;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual void shutdown() override;
                    virtual Scheduler::StageResult execute() override;
                
                private:
                    Engine* _engine;
                    std::weak_ptr<IRenderOutput> _output;
                    std::weak_ptr<RenderCommandContext> _cmdCtx;
            };

            virtual ~IRenderOutput() = default;

            virtual std::expected<void, Error> initialize(Engine& engine) = 0;
            virtual void shutdown() = 0;

            virtual std::expected<vk::Semaphore, Error> acquire(vk::Fence fence, uint64_t timeout = UINT64_MAX) = 0;
            virtual std::expected<void, Error> present(vk::Semaphore finishedSemaphore = {}) = 0;

            virtual uint32_t getCurrentBufferIndex() const = 0;
            virtual uint32_t getBufferCount() const = 0;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) = 0;
            virtual void end(vk::CommandBuffer cmd) = 0;

            /**
             * @brief Get if the previous 'acquire' call successfuly acquired the output
             */
            virtual bool wasAcquired() const = 0;


            virtual vk::Image currentColorImage(uint32_t attachment) const = 0;
            virtual vk::ImageView currentColorImageView(uint32_t attachment) const = 0;
            virtual vk::Image currentDepthStencilImage() const = 0;
            virtual vk::ImageView currentDepthStencilImageView() const = 0;

            virtual uint32_t colorAttachmentCount() const = 0;
            virtual bool hasDepthAttachment() const = 0;

            virtual uint64_t epoch() const = 0;
            
            virtual vk::Extent2D extent() const = 0;
            virtual vk::RenderPass renderPass() const = 0;
            inline virtual float scale() const {return 1.f;}
    };

    using SharedRenderOutput = std::shared_ptr<IRenderOutput>;
    using WeakRenderOutput = std::weak_ptr<IRenderOutput>;
    using RenderOutputMap = std::unordered_map<IRenderOutput::Name, SharedRenderOutput>;
}