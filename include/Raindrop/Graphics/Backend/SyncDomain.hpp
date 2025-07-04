#pragma once

#include "Raindrop/Graphics/Backend/CommandList.hpp"

namespace Raindrop::Graphics::Backend{
    class SyncDomain : public CommandList::Domain{
        public:
            virtual ~SyncDomain() override = default;
            
            virtual void pipelineBarrier() = 0;
            virtual void waitEvents() = 0;
            virtual void setEvent() = 0;
            virtual void resetEvent() = 0;
            virtual void writeTimestamp() = 0;
            virtual void beginQuery() = 0;
            virtual void endQuery() = 0;
            virtual void resetQueryPool() = 0;
            virtual void copyQueryPool() = 0;
    };
}