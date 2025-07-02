#pragma once

#include <memory>
#include "Buffer.hpp"
#include "CommandList.hpp"
#include "DescriptorSet.hpp"
#include "Framebuffer.hpp"
#include "GraphicsPipeline.hpp"
#include "RenderPass.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Raindrop::Graphics{
    class Engine;

    namespace Backend{
        class Device{
            public:
                virtual ~Device() = default;

                virtual std::shared_ptr<Buffer> createBuffer(const Buffer::Description& description) = 0;
                virtual std::shared_ptr<CommandList> createCommandList() = 0;
                virtual std::shared_ptr<DescriptorSet> createDescriptorSet() = 0;
                virtual std::shared_ptr<Framebuffer> createFramebuffer() = 0;
                virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline() = 0;
                virtual std::shared_ptr<RenderPass> createRenderPass() = 0;
                virtual std::shared_ptr<Sampler> createSampler() = 0;
                virtual std::shared_ptr<Shader> createShader() = 0;
                virtual std::shared_ptr<Texture> createTexture() = 0;
        };
    }
}