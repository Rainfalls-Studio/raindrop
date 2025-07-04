#include "Raindrop/Graphics/Backend/Vulkan/SyncDomain.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{
    SyncDomain::SyncDomain(VkCommandBuffer commandBuffer) : _commandBuffer{commandBuffer}{}
    SyncDomain::~SyncDomain(){}
    
    void SyncDomain::pipelineBarrier(){

    }

    void SyncDomain::waitEvents(){

    }

    void SyncDomain::setEvent(){

    }

    void SyncDomain::resetEvent(){

    }

    void SyncDomain::writeTimestamp(){

    }

    void SyncDomain::beginQuery(){

    }

    void SyncDomain::endQuery(){

    }

    void SyncDomain::resetQueryPool(){

    }

    void SyncDomain::copyQueryPool(){

    }
}