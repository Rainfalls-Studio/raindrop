#pragma once

#include "../QueueFamily.hpp"
#include "Context.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Queue.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/translation.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkQueueFlagBits toVulkan(Backend::QueueFamily::CapabilitiesBits&& bit){
        using enum Backend::QueueFamily::CapabilitiesBits;
        switch (bit){
            case NONE: return VkQueueFlagBits(0);
            case GRAPHICS: return VK_QUEUE_GRAPHICS_BIT;
            case COMPUTE: return VK_QUEUE_COMPUTE_BIT;
            case TRANSFER: return VK_QUEUE_TRANSFER_BIT;
            case VIDEO_DECODE: return VK_QUEUE_VIDEO_DECODE_BIT_KHR;
            case VIDEO_ENCODE: return VK_QUEUE_VIDEO_ENCODE_BIT_KHR;
            case SPARSE_BINDING: return VK_QUEUE_SPARSE_BINDING_BIT;
            default: break;
        }
        throw std::runtime_error("Undefined queue family capability");
    }

    RAINDROP_TO_VK_FLAG(VkQueueFlags, VkQueueFlagBits, Backend::QueueFamily::Capabilities)

    class QueueFamily : public Backend::QueueFamily {
        public:
            QueueFamily(Context& context, const VkQueueFamilyProperties& properties);
            virtual ~QueueFamily() override;

            virtual Capabilities getCapabilities() const override;
            virtual bool canPresent(std::shared_ptr<Surface> surface) const override;

            virtual uint32_t getQueueCount() const override;
            virtual std::shared_ptr<Backend::Queue> getQueue(uint32_t index) const override;

            uint32_t getFamilyIndex() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;
        
        private:
            static constexpr uint32_t INDEFINED_INDEX = ~static_cast<uint32_t>(0);

            Context& _context;
            uint32_t _index = INDEFINED_INDEX;
            std::vector<std::shared_ptr<Queue>> _queues;
            VkQueueFamilyProperties _properties;

            void queryQueues();
    };
}