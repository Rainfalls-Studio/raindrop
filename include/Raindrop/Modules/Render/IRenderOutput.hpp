#pragma once

#include <memory>
#include <unordered_map>

#include "RenderCoreModule.hpp"

namespace Raindrop::Render{
    class IRenderOutput{
        public:
            using Name = std::string;

            virtual ~IRenderOutput() = default;

            virtual void initialize(RenderCoreModule& engine) = 0;
            virtual void shutdown() = 0;

            // virtual void present(vk::Image image, vk::Semaphore readySemaphore = {});
    };

    using SharedRenderOutput = std::shared_ptr<IRenderOutput>;
    using WeakRenderOutput = std::weak_ptr<IRenderOutput>;
    using RenderOutputMap = std::unordered_map<IRenderOutput::Name, SharedRenderOutput>;
}