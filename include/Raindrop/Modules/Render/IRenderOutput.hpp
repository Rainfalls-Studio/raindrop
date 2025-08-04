#pragma once

#include <memory>
#include <unordered_map>

#include "RenderCoreModule.hpp"

namespace Raindrop::Render{
    class IRenderOutput{
        public:
            using Name = std::string;

            virtual ~IRenderOutput() = default;

            struct PreRenderResult{
                vk::Semaphore wait;
                vk::PipelineStageFlags waitStageFlags;
            };

            virtual void initialize(RenderCoreModule& engine) = 0;
            virtual void shutdown() = 0;

            virtual std::expected<bool, Error> acquire(uint64_t timeout = UINT64_MAX) = 0;
            virtual std::expected<PreRenderResult, Error> preRender(uint64_t timeout = UINT64_MAX) = 0;
            virtual std::expected<void, Error> present(vk::Semaphore waitSemaphore) = 0;

            virtual uint32_t getCurrentBufferIndex() const = 0;
            virtual uint32_t getBufferCount() const = 0;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) = 0;
            virtual void end(vk::CommandBuffer cmd) = 0;
    };

    using SharedRenderOutput = std::shared_ptr<IRenderOutput>;
    using WeakRenderOutput = std::weak_ptr<IRenderOutput>;
    using RenderOutputMap = std::unordered_map<IRenderOutput::Name, SharedRenderOutput>;
}