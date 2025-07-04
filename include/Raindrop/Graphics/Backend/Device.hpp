#pragma once

#include <memory>
#include "API.hpp"
#include "Buffer.hpp"
#include "CommandPool.hpp"
#include "DescriptorSet.hpp"
#include "Framebuffer.hpp"
#include "GraphicsPipeline.hpp"
#include "ComputePipeline.hpp"
#include "BufferView.hpp"
#include "DescriptorSetLayout.hpp"
#include "Fence.hpp"
#include "Image.hpp"
#include "ImageView.hpp"
#include "Raindrop/Graphics/Backend/AccelerationStructure.hpp"
#include "Raindrop/Graphics/Backend/DescriptorPool.hpp"
#include "Raindrop/Graphics/Backend/QueueFamily.hpp"
#include "Semaphore.hpp"
#include "TimelineSemaphore.hpp"
#include "RenderPass.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"

namespace Raindrop::Graphics{
    class Engine;

    namespace Backend{
        class Device{
            public:
                virtual ~Device() = default;

                virtual std::shared_ptr<Buffer> createBuffer(const Buffer::Description& description) = 0;
                virtual std::shared_ptr<BufferView> createBufferView(const BufferView::Description& description) = 0;

                virtual std::shared_ptr<Image> createImage(const Image::Description& description) = 0;
                virtual std::shared_ptr<ImageView> createImageView(const ImageView::Description& description) = 0;

                virtual std::shared_ptr<CommandPool> createCommandPool(const CommandPool::Description& description) = 0;

                virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const DescriptorSetLayout::Description& description) = 0;
                virtual std::shared_ptr<DescriptorPool> createDescriptorPool(const DescriptorPool::Description& description) = 0;
                virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSet::Description& description) = 0;

                virtual std::shared_ptr<Framebuffer> createFramebuffer(const Framebuffer::Description& description) = 0;
                virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline(const GraphicsPipeline::Description& description) = 0;
                virtual std::shared_ptr<ComputePipeline> createComputePipeline(const ComputePipeline::Description& description) = 0;
                virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPass::Description& description) = 0;
                virtual std::shared_ptr<Sampler> createSampler(const Sampler::Description& description) = 0;
                virtual std::shared_ptr<Shader> createShader(const Shader::Description& description) = 0;

                virtual std::shared_ptr<Fence> createFence(const Fence::Description& description) = 0;
                virtual std::shared_ptr<Semaphore> createSemaphore() = 0;
                virtual std::shared_ptr<TimelineSemaphore> createTimelineSemaphore(const TimelineSemaphore::Description& description) = 0;

                virtual std::vector<std::shared_ptr<QueueFamily>> getFamilies() const = 0;

                virtual void* getHandle() const noexcept = 0;
                virtual API getAPI() const noexcept = 0;
        };
    }
}