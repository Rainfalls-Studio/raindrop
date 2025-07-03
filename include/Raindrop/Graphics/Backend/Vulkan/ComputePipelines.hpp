#pragma once

#include "../ComputePipeline.hpp"
#include "Context.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    class ComputePipeline : public Backend::ComputePipeline{
        public:
            ComputePipeline(Context& context, const Description& computePipeline);
            virtual ~ComputePipeline() override;

            VkPipeline get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
    };
}