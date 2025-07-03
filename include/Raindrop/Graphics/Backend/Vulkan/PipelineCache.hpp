#pragma once

#include "../PipelineCache.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    class PipelineCache : public Backend::PipelineCache{
        public:
            PipelineCache(Context& context, const Description& description);
            virtual ~PipelineCache() override;

            VkPipelineCache get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            Context& _context;
            VkPipelineCache _pipelineCache;
    };
}