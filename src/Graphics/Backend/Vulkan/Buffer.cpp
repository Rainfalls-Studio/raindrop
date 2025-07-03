#include "Raindrop/Graphics/Backend/Vulkan/Buffer.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	Buffer::Buffer(Context& context, const Description& buffer) :
        _context{context}
    {
        
    }

    Buffer::~Buffer(){
        if (_buffer){
            vkDestroyBuffer(_context.device, _buffer, nullptr);
            _buffer = VK_NULL_HANDLE;
        }
    }


    void* Buffer::map(Size size, Size offset){
        // vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void **ppData)
    }

    void Buffer::unmap(){

    }

    void Buffer::flush(Size size, Size offset){

    }

    void Buffer::invalidate(Size size, Size offset){

    }

    void* Buffer::getHandle() const noexcept{
        return static_cast<void*>(_buffer);
    }

    API Buffer::getAPI() const noexcept{
        return API::VULKAN;
    }
}