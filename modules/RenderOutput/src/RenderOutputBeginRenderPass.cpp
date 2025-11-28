#include "Raindrop/Modules/Render/RenderOutput/RenderOutputBeginRenderPass.hpp"

#include <RenderGraph/GraphContext.hpp>
#include <RenderGraph/ImageData.hpp>
#include <RenderGraph/RunnableGraph.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Render{
	RenderOutputBeginRenderPass::RenderOutputBeginRenderPass(SharedRenderOutput renderOutput, std::shared_ptr<RenderGraph> renderGraph) : 
        _renderOutput{renderOutput},
        _renderGraph{renderGraph} 
	{}

    void RenderOutputBeginRenderPass::record(vk::CommandBuffer cmd){
        if (auto graph = _renderGraph.lock()) graph->wantBufferCount(_renderOutput->getBufferCount());
        _renderOutput->begin(cmd);
    }
}