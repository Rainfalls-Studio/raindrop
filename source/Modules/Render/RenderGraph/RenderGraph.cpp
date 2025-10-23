#include "Raindrop/Modules/Render/RenderGraph/RenderGraph.hpp"
#include "Raindrop/Modules/Render/RenderGraph/RenderGraphModule.hpp"
#include "Raindrop/Modules/Render/Core/RenderCoreModule.hpp"
#include <RenderGraph/RunnableGraph.hpp>

namespace Raindrop::Render{
    RenderGraph::RenderGraph(std::shared_ptr<RenderGraphModule> module, const std::string& name) :
        _module{module}
    {
        if (!module){
            spdlog::error("The module is not valid !");
            return;
        }

        _frameGraph = std::make_shared<crg::FrameGraph>(module->resourceHandler(), name);
    }

    RenderGraph::~RenderGraph(){

    }

    void RenderGraph::invalidate(){
        _pendingRecompile = true;
    }

    void RenderGraph::wantBufferCount(uint32_t count){

        // can't handle dynamic dealocation for now It's hard to check for used resources
        _wantedBufferCount = std::max(_wantedBufferCount, count);
    }

    RenderGraph::SyncResult RenderGraph::run(vk::Queue queue, vk::Semaphore toWait, vk::PipelineStageFlags semaphoreWaitStage){

        auto module = _module.lock();

        if (!module){
            spdlog::warn("the render graph has a non valid render graph module");
            return SyncResult{.success=false};
        }

        // handle resize request
        if (_wantedBufferCount != _bufferCount){
            _compiledGraphs.resize(_wantedBufferCount);
            _bufferCount = _wantedBufferCount;
        }
        
        auto& graph = _compiledGraphs[_currentBuffer];
        auto& runnableGraph = graph.graph;

        if (graph.pendingRecompile){
            runnableGraph = _frameGraph->compile(module->context());
            graph.pendingRecompile = false;
        }

        auto core = module->core();

        auto renderFinished = runnableGraph->run(
            {crg::SemaphoreWait{toWait, VkPipelineStageFlags(semaphoreWaitStage)}},
            queue
        );

        return SyncResult{
            .fence = vk::Fence(runnableGraph->getFence()),
            .semaphore = renderFinished[0].semaphore
        };
    }

    std::string RenderGraph::name() const{
        return _frameGraph->getName();
    }

    uint32_t RenderGraph::bufferCount() const{
        return _bufferCount;
    }

    uint32_t RenderGraph::currentBuffer() const{
        return _currentBuffer;
    }

    crg::FrameGraph& RenderGraph::get(){
        return *_frameGraph;
    }

}