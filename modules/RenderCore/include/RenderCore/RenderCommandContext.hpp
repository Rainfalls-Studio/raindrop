#pragma once

#include <vector>
#include <expected>

#include <vulkan/vulkan.hpp>

#include "RenderCoreModule.hpp"

#include <Scheduler/IStage.hpp>
#include <Raindrop/Error.hpp>

namespace Raindrop::Render{
    class RenderCommandContext{
        public:
            enum class ErrorCode{
                UNKNOWN_QUEUE_TYPE,
                FAILED_POOL_CREATION,
                FAILED_COMMAND_BUFFER_ALLOCATION,
                FAILED_FENCE_CREATION,
                FAILED_SEMAPHORE_CREATION,
                ALREADY_RECORDING,
                NOT_RECORDING,
                FAILED_BEGIN_RECORD,
                FAILED_END_RECORD
            };

            static std::error_category& error_category();

            static inline std::error_code make_error_code(ErrorCode e){
                return {static_cast<int>(e), error_category()};
            }

            static inline std::error_code UnknownQueueTypeError() {return make_error_code(ErrorCode::UNKNOWN_QUEUE_TYPE);}
            static inline std::error_code FailedPoolCreationError() {return make_error_code(ErrorCode::FAILED_POOL_CREATION);}
            static inline std::error_code FailedCommandBufferAllocationError() {return make_error_code(ErrorCode::FAILED_COMMAND_BUFFER_ALLOCATION);}
            static inline std::error_code FailedFenceCreationError() {return make_error_code(ErrorCode::FAILED_FENCE_CREATION);}
            static inline std::error_code FailedSemaphoreCreationError() {return make_error_code(ErrorCode::FAILED_SEMAPHORE_CREATION);}
            static inline std::error_code AlreadyRecordingError() {return make_error_code(ErrorCode::ALREADY_RECORDING);}
            static inline std::error_code NotRecordingError() {return make_error_code(ErrorCode::ALREADY_RECORDING);}
            static inline std::error_code FailedBeginRecordError() {return make_error_code(ErrorCode::FAILED_BEGIN_RECORD);}
            static inline std::error_code FailedEndRecordError() {return make_error_code(ErrorCode::FAILED_END_RECORD);}

            class BeginStage : public Scheduler::IStage{
                public:
                    BeginStage(const std::shared_ptr<RenderCommandContext>& ctx);
                    virtual ~BeginStage() override;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual Scheduler::StageResult execute() override;
                
                private:
                    std::weak_ptr<RenderCommandContext> _ctx;
            };

            class EndStage : public Scheduler::IStage{
                public:
                    EndStage(const std::shared_ptr<RenderCommandContext>& ctx);
                    virtual ~EndStage() override;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual Scheduler::StageResult execute() override;
                
                private:
                    std::weak_ptr<RenderCommandContext> _ctx;
            };

            class SubmitStage : public Scheduler::IStage{
                public:
                    SubmitStage(const std::shared_ptr<RenderCommandContext>& ctx, Queue queue);
                    virtual ~SubmitStage() override;

                    virtual const char* name() const override;

                    virtual void initialize(Scheduler::StageInitHelper& helper) override;
                    virtual Scheduler::StageResult execute() override;
                
                private:
                    std::weak_ptr<RenderCommandContext> _ctx;
                    Queue _queue;
            };

            struct SemaphoreWait{
                vk::Semaphore semaphore;
                vk::PipelineStageFlags stage;
            };

            struct Buffer{
                vk::CommandBuffer cmdBuffer;
                vk::Semaphore semaphore;
                vk::Fence fence;
                bool inUse = false;
            };

            RenderCommandContext(std::shared_ptr<RenderCoreModule> core, Queue::Type type, uint32_t bufferCount = 1, const std::string& name = {});
            ~RenderCommandContext();

            std::expected<vk::CommandBuffer, Error> begin();
            std::expected<void, Error> end();
            void nextFrame();

            uint32_t currentIndex() const;
            uint32_t bufferCount() const;

            Buffer& currentBuffer();

            void waitFor(vk::Semaphore semaphore, vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eBottomOfPipe);
            void skipFrame();

            std::vector<SemaphoreWait>& semaphoreWaits();
            bool SkipingFrame() const;

            std::shared_ptr<RenderCoreModule> core();

        private:
            

            const std::string _name;
            Queue::Type _type;
            std::shared_ptr<RenderCoreModule> _core;

            vk::CommandPool _pool;
            std::vector<Buffer> _buffers;

            std::vector<SemaphoreWait> _waitSemaphores;

            uint32_t _currentIndex = 0;
            uint32_t _wantedBufferCount = 1;
            uint32_t _bufferCount;
            bool _recording = false;
            bool _skipFrame = false;

            std::expected<void, Error> createPool();
            void destroyPool();
            std::expected<void, Error> allocateBuffers(uint32_t n);
            std::expected<void, Error> createFences();
            std::expected<void, Error> createSemaphores();

            void freeCommandBuffers();
            void destroyFences();
            void destroySemaphores();

            void waitForFences();
    };
}