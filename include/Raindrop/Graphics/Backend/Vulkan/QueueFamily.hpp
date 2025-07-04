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
    VkQueueFlagBits toVulkan(Backend::QueueFamily::CapabilitiesBits&& bit);

    template<>
    VkQueueFlags toVulkan(Backend::QueueFamily::Capabilities&& bit);


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