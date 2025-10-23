#include "Raindrop/Modules/Render/RenderOutput/RenderOutputBeginRenderPass.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/ImageData.hpp>
#include <RenderGraph/RunnableGraph.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
	RenderOutputBeginRenderPass::RenderOutputBeginRenderPass(SharedRenderOutput renderOutput) : 
        _renderOutput{renderOutput}
	{}

    void RenderOutputBeginRenderPass::record(vk::CommandBuffer cmd){
        _renderOutput->begin(cmd);
    }
}