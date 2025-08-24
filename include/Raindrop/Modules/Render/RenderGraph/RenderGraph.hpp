#pragma once

#include <memory>
#include <vector>

#include <RenderGraph/FrameGraphPrerequisites.hpp>


namespace Raindrop::Render{
    struct RenderGraph{
        std::shared_ptr<crg::FrameGraph> frameGraph;

        void invalidate();

        bool pendingRecompile = true;
        uint32_t bufferCount = 2;
    };
}