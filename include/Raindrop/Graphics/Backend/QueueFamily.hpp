#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"
#include "API.hpp"
#include "Queue.hpp"
#include "Raindrop/Graphics/Backend/Surface.hpp"

namespace Raindrop::Graphics::Backend{
    class QueueFamily{
        public:
            enum class CapabilitiesBits{
                NONE = 0,
                GRAPHICS = 1 << 0,
                COMPUTE = 1 << 1,
                TRANSFER = 1 << 2,
                VIDEO_DECODE = 1 << 4,
                VIDEO_ENCODE = 1 << 5,
                SPARSE_BINDING = 1 << 6,
                __SIZE__
            };

            RAINDROP_FLAG_CLASS(Capabilities, CapabilitiesBits)

            virtual ~QueueFamily() = default;
            
            virtual Capabilities getCapabilities() const = 0;
            virtual bool canPresent(std::shared_ptr<Surface> surface) const = 0;

            virtual uint32_t getQueueCount() const = 0;
            virtual std::shared_ptr<Queue> getQueue(uint32_t index) const = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}