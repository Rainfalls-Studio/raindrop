#include "Raindrop/Graphics/Backend/Vulkan/TransferDomain.hpp"

namespace Raindrop::Graphics::Backend::Vulkan{

    TransferDomain::TransferDomain(VkCommandBuffer commandBuffer) : _commandBuffer{commandBuffer}{}
    TransferDomain::~TransferDomain(){}

    void TransferDomain::copyBuffer(){

    }

    void TransferDomain::fillBuffer(){

    }

    void TransferDomain::updateBuffer(){

    }
    
    void TransferDomain::copyBufferToImage(){

    }

    void TransferDomain::copyImageToBuffer(){

    }

    void TransferDomain::copyImage(){

    }

    void TransferDomain::blitImage(){

    }

    void TransferDomain::resolveImage(){

    }


    void TransferDomain::clearColorImage(){

    }

    void TransferDomain::clearDepthStencilImage(){

    }
}