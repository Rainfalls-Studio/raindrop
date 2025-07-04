#pragma once

#include "Raindrop/Graphics/Backend/GraphicsDomain.hpp"
#include "Raindrop/Graphics/Backend/SubpassContent.hpp"
#include "translation.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkSubpassContents toVulkan(SubpassContent&& content);

    
    class GraphicsDomain : public Backend::GraphicsDomain{
        public: 
            GraphicsDomain(VkCommandBuffer commandBuffer);
            virtual ~GraphicsDomain() override;

            virtual void beginRenderPass(const RenderPassBeginInfo& info) override;
            virtual void endRenderPass() override;
            virtual void nextSubpass(SubpassContent content) override;
            virtual void beginRendering(const RenderingInfo& info) override;
            virtual void endRendering() override;
            virtual void beginConditionalRendering() override;
            virtual void endConditionalRendering() override;
            
            virtual void bindPipeline(std::shared_ptr<GraphicsPipeline> pipeline) override;
            virtual void bindDescriptorSets(std::shared_ptr<PipelineLayout> layout) override;
            virtual void bindVertexBuffers() override;
            virtual void bindIndexBuffer() override;
            virtual void pushConstants() override;
            virtual void setViewport() override;
            virtual void setLineWidth() override;
            virtual void setDepthBias() override;
            virtual void setBlendConstants() override;
            virtual void setDepthBounds() override;
            virtual void setStencilCompareMask() override;
            virtual void setStencilReference() override;
            virtual void setCullMode() override;
            virtual void setFrontFace() override;
            virtual void setPrimitiveTopology() override;
            virtual void setViewports() override;
            virtual void setScissors() override;

            virtual void draw() override;
            virtual void drawIndexed() override;
            virtual void drawIndirect() override;
            virtual void drawIndexedIndirect() override;

            virtual void clearAttachments() override;
        
        private:
            VkCommandBuffer _commandBuffer;
    };
}