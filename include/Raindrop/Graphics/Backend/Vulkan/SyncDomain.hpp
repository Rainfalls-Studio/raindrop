#pragma once

#include "../SyncDomain.hpp"
#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{

    class SyncDomain : public Backend::SyncDomain{
        public: 
            SyncDomain(VkCommandBuffer commandBuffer);
            virtual ~SyncDomain() override;
            
            virtual void pipelineBarrier() override;
            virtual void waitEvents() override;
            virtual void setEvent() override;
            virtual void resetEvent() override;
            virtual void writeTimestamp() override;
            virtual void beginQuery() override;
            virtual void endQuery() override;
            virtual void resetQueryPool() override;
            virtual void copyQueryPool() override;

        private:
            VkCommandBuffer _commandBuffer;
    };
}