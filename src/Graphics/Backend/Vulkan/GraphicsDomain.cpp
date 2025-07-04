#include "Raindrop/Graphics/Backend/Vulkan/GraphicsDomain.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/RenderPass.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <cassert>

namespace Raindrop::Graphics::Backend::Vulkan{
    void GraphicsDomain::beginRenderPass(const RenderPassBeginInfo& info){
        assert(info.renderPass && "renderPass must be valid");
        assert(info.renderPass->getAPI() == API::VULKAN && "renderPass must be from the vulkan API");
        assert(info.framebuffer && "framebuffer must be valid");
        assert(info.framebuffer->getAPI() == API::VULKAN && "The framebuffer must be from the vulkan API");

        std::vector<VkClearValue> clearValues(info.clearValues.size());

        for (uint32_t i=0; i<clearValues.size(); i++){
            auto& dst = clearValues[i];
            auto& src = info.clearValues[i];

            dst.color.uint32[0] = src.color.uint32.r;
            dst.color.uint32[1] = src.color.uint32.g;
            dst.color.uint32[2] = src.color.uint32.b;
            dst.color.uint32[3] = src.color.uint32.a;
            dst.depthStencil.depth = src.depthStencil.depth;
            dst.depthStencil.stencil = src.depthStencil.stencil;
        }

        VkRenderPassBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        beginInfo.renderPass = std::static_pointer_cast<RenderPass>(info.renderPass)->get();
        beginInfo.clearValueCount = static_cast<uint32_t>(info.clearValues.size());
        beginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(
            _commandBuffer,
            &beginInfo,
            toVulkan<VkSubpassContents>(info.subpassContents)
        );
    }

    void GraphicsDomain::endRenderPass(){
        vkCmdEndRenderPass(_commandBuffer);
    }

    void GraphicsDomain::nextSubpass(SubpassContent content){
        vkCmdNextSubpass(_commandBuffer, toVulkan<VkSubpassContents>(content));
    }

    void GraphicsDomain::beginRendering(const RenderingInfo& info){

    }

    void GraphicsDomain::endRendering(){

    }

    void GraphicsDomain::beginConditionalRendering(){

    }

    void GraphicsDomain::endConditionalRendering(){

    }

    
    void GraphicsDomain::bindPipeline(std::shared_ptr<GraphicsPipeline> pipeline){

    }

    void GraphicsDomain::bindDescriptorSets(std::shared_ptr<PipelineLayout> layout){

    }

    void GraphicsDomain::bindVertexBuffers(){

    }

    void GraphicsDomain::bindIndexBuffer(){

    }

    void GraphicsDomain::pushConstants(){

    }

    void GraphicsDomain::setViewport(){

    }

    void GraphicsDomain::setLineWidth(){

    }

    void GraphicsDomain::setDepthBias(){

    }

    void GraphicsDomain::setBlendConstants(){

    }

    void GraphicsDomain::setDepthBounds(){

    }

    void GraphicsDomain::setStencilCompareMask(){

    }

    void GraphicsDomain::setStencilReference(){

    }

    void GraphicsDomain::setCullMode(){

    }

    void GraphicsDomain::setFrontFace(){

    }

    void GraphicsDomain::setPrimitiveTopology(){

    }

    void GraphicsDomain::setViewports(){

    }

    void GraphicsDomain::setScissors(){

    }


    void GraphicsDomain::draw(){

    }

    void GraphicsDomain::drawIndexed(){

    }

    void GraphicsDomain::drawIndirect(){

    }

    void GraphicsDomain::drawIndexedIndirect(){

    }


    void GraphicsDomain::clearAttachments(){

    }

}