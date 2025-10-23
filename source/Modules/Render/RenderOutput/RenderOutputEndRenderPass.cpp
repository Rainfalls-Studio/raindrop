#include "Raindrop/Modules/Render/RenderOutput/RenderOutputEndRenderPass.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/ImageData.hpp>
#include <RenderGraph/RunnableGraph.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
	RenderOutputEndRenderPass::RenderOutputEndRenderPass(SharedRenderOutput renderOutput) : 
        _renderOutput{renderOutput}
	{}

    void RenderOutputEndRenderPass::record(vk::CommandBuffer cmd){
        _renderOutput->end(cmd);
    }
}