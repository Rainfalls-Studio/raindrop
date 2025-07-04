#pragma once

#include "../TransferDomain.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{

    class TransferDomain : public Backend::TransferDomain{
        public: 
            TransferDomain(VkCommandBuffer commandBuffer);
            virtual ~TransferDomain() override;

            virtual void copyBuffer() override;
            virtual void fillBuffer() override;
            virtual void updateBuffer() override;
            
            virtual void copyBufferToImage() override;
            virtual void copyImageToBuffer() override;

            virtual void copyImage() override;
            virtual void blitImage() override;
            virtual void resolveImage() override;

            virtual void clearColorImage() override;
            virtual void clearDepthStencilImage() override;
        
        private:
            VkCommandBuffer _commandBuffer;
    };
}