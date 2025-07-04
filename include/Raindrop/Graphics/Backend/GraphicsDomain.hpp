#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "CommandList.hpp"
#include "Raindrop/Core/Utils/Flags.hpp"
#include "Raindrop/Graphics/Backend/GraphicsPipeline.hpp"
#include "SubpassContent.hpp"
#include "RenderPass.hpp"
#include "ImageView.hpp"
#include "Framebuffer.hpp"
#include "PipelineLayout.hpp"

namespace Raindrop::Graphics::Backend{
    enum class RenderingFlagBits{
        NONE = 0,
        SECONDARY = 1 << 0,
        SUSPENDING = 1 << 1,
        RESUMING = 1 << 2,
        __SIZE__
    };

    RAINDROP_FLAG(RenderingFlags, RenderingFlagBits)

    enum class ResolveMode{
        NONE,
        SAMPLE_ZERO,
        AVERAGE,
        MIN,
        MAX
    };

    enum class StoreOp{
        NONE = 0,
        STORE = 1,
        DONT_CARE = 2
    };
    
    enum class LoadOp{
        NONE = 0,
        LOAD = 1,
        CLEAR = 2,
        DONT_CARE = 3
    };

    struct ClearColorValue{
        union{
            struct {
                float r, g, b, a;
            } float32;

            struct {
                int32_t r, g, b, a;
            } int32;

            struct {
                uint32_t r, g, b, a;
            } uint32;
        };
    };

    struct ClearDepthStencilValue{
        float depth;
        uint32_t stencil;
    };

    struct ClearValue{
        ClearColorValue color;
        ClearDepthStencilValue depthStencil;
    };

    struct RenderingAttachment{
        std::shared_ptr<ImageView> imageView;
        Image::Layout layout;
        ResolveMode resolveMode;
        std::shared_ptr<ImageView> resolveImageView;
        Image::Layout resolveImageLayout;
        ClearValue clearValue;
    };

    struct RenderingInfo{
        RenderingFlags flags;
        // Rect2D renderArea
        uint32_t layerCount;
        uint32_t viewMask;

        std::vector<RenderingAttachment> colorAttachments;
        std::optional<RenderingAttachment> depthAttachment;
        std::optional<RenderingAttachment> stencilAttachment;
    };

    struct RenderPassBeginInfo{
        std::shared_ptr<RenderPass> renderPass;
        std::shared_ptr<Framebuffer> framebuffer;
        std::vector<ClearValue> clearValues;
        SubpassContent subpassContents;
    };

    class GraphicsDomain : public CommandList::Domain{
        public:
            virtual ~GraphicsDomain() override = default;

            virtual void beginRenderPass(const RenderPassBeginInfo& info) = 0;
            virtual void endRenderPass() = 0;
            virtual void nextSubpass(SubpassContent content) = 0;
            virtual void beginRendering(const RenderingInfo& info) = 0;
            virtual void endRendering() = 0;
            virtual void beginConditionalRendering() = 0;
            virtual void endConditionalRendering() = 0;
            
            virtual void bindPipeline(std::shared_ptr<GraphicsPipeline> pipeline) = 0;
            virtual void bindDescriptorSets(std::shared_ptr<PipelineLayout> layout) = 0;
            virtual void bindVertexBuffers() = 0;
            virtual void bindIndexBuffer() = 0;
            virtual void pushConstants() = 0;
            virtual void setViewport() = 0;
            virtual void setLineWidth() = 0;
            virtual void setDepthBias() = 0;
            virtual void setBlendConstants() = 0;
            virtual void setDepthBounds() = 0;
            virtual void setStencilCompareMask() = 0;
            virtual void setStencilReference() = 0;
            virtual void setCullMode() = 0;
            virtual void setFrontFace() = 0;
            virtual void setPrimitiveTopology() = 0;
            virtual void setViewports() = 0;
            virtual void setScissors() = 0;

            virtual void draw() = 0;
            virtual void drawIndexed() = 0;
            virtual void drawIndirect() = 0;
            virtual void drawIndexedIndirect() = 0;

            virtual void clearAttachments() = 0;
    };
}