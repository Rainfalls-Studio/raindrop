#pragma once

#include <memory>
#include <vector>

#include <RenderGraph/FrameGraphPrerequisites.hpp>


namespace Raindrop::Render{
    struct RenderGraph{
        std::shared_ptr<crg::FrameGraph> _frameGraph;

        void invalidate();

        bool _pendingRecompile = true;
        uint32_t _bufferCount = 2;
    };
}