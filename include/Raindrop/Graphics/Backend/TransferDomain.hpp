#pragma once

#include "Raindrop/Graphics/Backend/CommandList.hpp"

namespace Raindrop::Graphics::Backend{
    class TransferDomain : public CommandList::Domain{
        public:
            virtual ~TransferDomain() override = default;

            virtual void copyBuffer() = 0;
            virtual void fillBuffer() = 0;
            virtual void updateBuffer() = 0;
            
            virtual void copyBufferToImage() = 0;
            virtual void copyImageToBuffer() = 0;

            virtual void copyImage() = 0;
            virtual void blitImage() = 0;
            virtual void resolveImage() = 0;

            virtual void clearColorImage() = 0;
            virtual void clearDepthStencilImage() = 0;
    };
}