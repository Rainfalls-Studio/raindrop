#pragma once

#include <memory>
#include "Buffer.hpp"
#include "CommandList.hpp"
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

                virtual std::shared_ptr<CommandList> createCommandList() = 0;

                virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout() = 0;
                virtual std::shared_ptr<DescriptorPool> createDescriptorPool() = 0;
                virtual std::shared_ptr<DescriptorSet> createDescriptorSet() = 0;

                virtual std::shared_ptr<Framebuffer> createFramebuffer() = 0;
                virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline() = 0;
                virtual std::shared_ptr<ComputePipeline> createComputePipeline() = 0;
                virtual std::shared_ptr<RenderPass> createRenderPass() = 0;
                virtual std::shared_ptr<Sampler> createSampler(const Shader::Description& description) = 0;
                virtual std::shared_ptr<Shader> createShader() = 0;

                virtual std::shared_ptr<AccelerationStructure> createAccelerationStructure() = 0;

                virtual std::shared_ptr<Fence> createFence(const Fence::Description& description) = 0;
                virtual std::shared_ptr<Semaphore> createSemaphore() = 0;
                virtual std::shared_ptr<TimelineSemaphore> createTimelineSemaphore(const TimelineSemaphore::Description& description) = 0;
        };
    }
}