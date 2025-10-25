#pragma once

#include <memory>
#include <vector>

// #include <RenderGraph/FrameGraphPrerequisites.hpp>
#include <RenderGraph/FrameGraph.hpp>
#include <vulkan/vulkan.hpp>


namespace Raindrop::Render{
    class RenderGraphModule;
    class RenderGraph{
        public:
            struct SyncResult{
                bool success = true;
                vk::Fence fence = {};
                vk::Semaphore semaphore = {};
            };

            RenderGraph(std::shared_ptr<RenderGraphModule> module, const std::string& name);
            ~RenderGraph();

            void invalidate();
            void wantBufferCount(uint32_t count);

            SyncResult run(vk::Queue queue, vk::Semaphore toWait, vk::PipelineStageFlags semaphoreWaitStage = vk::PipelineStageFlagBits::eBottomOfPipe);

            inline crg::FrameGraph* operator->(){
                return &get();
            }

            inline crg::FrameGraph& operator*(){
                return get();
            }

            crg::FrameGraph& get();

            std::string name() const;
            uint32_t bufferCount() const;
            uint32_t currentBuffer() const;

        private:
            std::weak_ptr<RenderGraphModule> _module;
            std::shared_ptr<crg::FrameGraph> _frameGraph;

            struct CompiledGraph{
                crg::RunnableGraphPtr graph;
                bool pendingRecompile = true;
            };
            std::vector<CompiledGraph> _compiledGraphs;

            uint32_t _bufferCount = 0;
            uint32_t _currentBuffer = 0;

            uint32_t _wantedBufferCount = 1;

            void compileGraph(uint32_t index);
    };
}