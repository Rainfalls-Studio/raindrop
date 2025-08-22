#include "Raindrop/Modules/Render/RenderOutput/BlitToRenderOutputPass.hpp"
#include "Raindrop/Modules/Render/RenderOutput/RenderOutputModule.hpp"
#include "Raindrop/Engine.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/ImageData.hpp>
#include <RenderGraph/RunnableGraph.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
	BlitToRenderOutputPass::BlitToRenderOutputPass(
        Engine& engine,
        const crg::FramePass& pass,
		crg::GraphContext& context,
		crg::RunnableGraph& graph,
        const IRenderOutput::Name& renderOutput,
        crg::ru::Config ruConfig,
        GetPassIndexCallback passIndex) : RunnablePass{
            pass,
            context,
            graph,
            {
                InitialiseCallback([this](uint32_t index){initialize(index);}),
                GetPipelineStateCallback( [](){ return crg::getPipelineState(VK_PIPELINE_STAGE_TRANSFER_BIT ); } ),
                [this]( crg::RecordContext & recContext, VkCommandBuffer cb, uint32_t i ){ recordInto( recContext, cb, i ); },
                std::move( passIndex ),
                IsEnabledCallback([this]{return isEnabled();})
            },
            std::move( ruConfig )
        },
        _engine{&engine},
        _renderOutputName{renderOutput}
	{
        
	}

    void BlitToRenderOutputPass::recordInto(
        crg::RecordContext& context,
        VkCommandBuffer commandBuffer,
        uint32_t index [[maybe_unused]]
    ){
        _renderOutputLock->begin(commandBuffer);

        #if 0
		auto srcView =  m_pass.images.front().view(index);
		auto srcImage = m_graph.createImage(srcView.data->image);

        vk::Image dstImage = _renderOutputLock->image();
        VkImageSubresourceRange range = srcView.data->info.subresourceRange;

        vk::Extent3D srcExtent = srcView.data->image.data->info.extent;
        vk::Extent3D dstExtent = srcView.data->image.data->info.extent;

        VkImageBlit blitRegion = {
            VkImageSubresourceLayers{
                range.aspectMask,
                range.baseMipLevel,
                range.baseArrayLayer,
                1
            },
            {
                {0, 0, 0},
                {static_cast<int32_t>(srcExtent.width), static_cast<int32_t>(srcExtent.height), 0}
            },
            VkImageSubresourceLayers{
                0,
                0,
                0,
                1
            },
            {
                {0, 0, 0},
                {static_cast<int32_t>(dstExtent.width), static_cast<int32_t>(dstExtent.height), 0}
            }
        };

        context->vkCmdBlitImage(
            commandBuffer,
            srcImage,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            dstImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &blitRegion,
            VK_FILTER_NEAREST
        );
        #else

        vk::Image dstImage = _renderOutputLock->image();

        VkClearColorValue clearColor = { .float32 = {1.f, 0., 0., 0. } };

        VkImageSubresourceRange range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        context->vkCmdClearColorImage(
            commandBuffer,
            dstImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            &clearColor,
            1,
            &range
        );

        #endif

        _renderOutputLock->end(commandBuffer);
    }

    void BlitToRenderOutputPass::initialize(uint32_t){
        _renderOutputLock = findRenderOutput();
    }

    bool BlitToRenderOutputPass::isEnabled(){
        _renderOutputLock = findRenderOutput();
        return _renderOutputLock != nullptr;
    }

    SharedRenderOutput BlitToRenderOutputPass::findRenderOutput(){
        auto module = _engine->getModuleManager().getModuleAs<RenderOutputModule>("RenderOutput");

        if (!module){
            spdlog::error("Can't find render output \"{}\" as the render output module is not registred !", _renderOutputName);
            return nullptr;
        }

        auto output = module->getOutput(_renderOutputName);

        if (!output){
            spdlog::error("Can't find render output \"{}\"", _renderOutputName);
        }

        return output;
    }
}