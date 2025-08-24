#pragma once

#include <memory>
#include <unordered_map>

#include "../Core/RenderCoreModule.hpp"
#include "Raindrop/Core/Store/Resource.hpp"
#include "RenderOutputResource.hpp"

namespace Raindrop::Render{
    class IRenderOutput{
        public:
            using Name = std::string;

            virtual ~IRenderOutput() = default;

            virtual void initialize(Engine& engine) = 0;
            virtual void shutdown() = 0;

            virtual std::expected<bool, Error> acquire(uint64_t timeout = UINT64_MAX) = 0;
            virtual std::expected<void, Error> preRender(uint64_t timeout = UINT64_MAX) = 0;
            virtual std::expected<void, Error> postRender() = 0;

            virtual uint32_t getCurrentBufferIndex() const = 0;
            virtual uint32_t getBufferCount() const = 0;

            virtual void begin(vk::CommandBuffer cmd, vk::SubpassContents subpassContents = vk::SubpassContents::eInline) = 0;
            virtual void end(vk::CommandBuffer cmd) = 0;

            virtual vk::Image image() const = 0;
            virtual vk::Extent2D extent() const = 0;

            virtual Store::ResourcePtr<RenderOutputResource> resources() = 0;
    };

    using SharedRenderOutput = std::shared_ptr<IRenderOutput>;
    using WeakRenderOutput = std::weak_ptr<IRenderOutput>;
    using RenderOutputMap = std::unordered_map<IRenderOutput::Name, SharedRenderOutput>;
}