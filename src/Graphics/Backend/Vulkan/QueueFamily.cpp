#include "Raindrop/Graphics/Backend/Vulkan/QueueFamily.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Queue.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Surface.hpp"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <stdexcept>

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
    
    QueueFamily::QueueFamily(Context& context, const VkQueueFamilyProperties& properties) : 
        _context{context},
        _properties{properties}
    {
        queryQueues();
    }
        
    QueueFamily::~QueueFamily(){}

    void QueueFamily::queryQueues(){
        auto& device = _context.device;

        _queues.resize(_properties.queueCount);
        for (uint32_t i=0; i<_properties.queueCount; i++){
            VkQueue queue = VK_NULL_HANDLE;

            vkGetDeviceQueue(
                device,
                _index,
                i,
                &queue
            );

            _queues[i] = std::make_shared<Queue>(_context, queue);
        }
    }
        
    QueueFamily::Capabilities QueueFamily::getCapabilities() const {
        Capabilities capabilities;
        auto& queueFlags = _properties.queueFlags;

        if (queueFlags & VK_QUEUE_GRAPHICS_BIT) capabilities |= Capabilities::GRAPHICS;
        if (queueFlags & VK_QUEUE_COMPUTE_BIT) capabilities |= Capabilities::COMPUTE;
        if (queueFlags & VK_QUEUE_TRANSFER_BIT) capabilities |= Capabilities::TRANSFER;
        if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) capabilities |= Capabilities::VIDEO_DECODE;
        if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) capabilities |= Capabilities::VIDEO_ENCODE;
        if (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) capabilities |= Capabilities::SPARSE_BINDING;

        return capabilities;
    }

    bool QueueFamily::canPresent(std::shared_ptr<Backend::Surface> surface) const{
        assert(surface && "A valid surface is required");
        assert(surface->getAPI() == API::VULKAN && "Surface API must be Vulkan");

        QueueFamily::Capabilities capabilities;

        VkBool32 supported = VK_FALSE;
        if (vkGetPhysicalDeviceSurfaceSupportKHR(
            _context.physicalDevice,
            _index, 
            std::static_pointer_cast<Surface>(surface)->get(),
            &supported
        ) != VK_SUCCESS){
            throw std::runtime_error("Failed to query surface support for queue family");
        }

        return static_cast<bool>(supported);
    }

    uint32_t QueueFamily::getQueueCount() const{
        return _properties.queueCount;
    }

    std::shared_ptr<Backend::Queue> QueueFamily::getQueue(uint32_t index) const{
        assert(index < _queues.size() && "A valid index is required");
        return std::static_pointer_cast<Backend::Queue>(_queues[index]);
    }

    uint32_t QueueFamily::getFamilyIndex() const noexcept{
        return _index;
    }

    void* QueueFamily::getHandle() const noexcept{
        return reinterpret_cast<void*>(_index);
    }

    API QueueFamily::getAPI() const noexcept{
        return API::VULKAN;
    }
}