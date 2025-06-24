#pragma once

#include "../SurfaceProvider.hpp"
#include <cstdint>
#include <string_view>

/* Avoid including vulkan.h, don't define VkInstance if it's already included */
#ifdef VULKAN_H_
    #define NO_SDL_VULKAN_TYPEDEFS
#endif

#ifndef NO_SDL_VULKAN_TYPEDEFS
    #define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;

    #if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
        #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef struct object##_T *object;
    #else
        #define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef uint64_t object;
    #endif

    VK_DEFINE_HANDLE(VkInstance)
    VK_DEFINE_HANDLE(VkPhysicalDevice)
    VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkSurfaceKHR)
    struct VkAllocationCallbacks;

    /* Make sure to undef to avoid issues in case of later vulkan include */
    #undef VK_DEFINE_HANDLE
    #undef VK_DEFINE_NON_DISPATCHABLE_HANDLE

#endif /* !NO_SDL_VULKAN_TYPEDEFS */

namespace Raindrop::Window::SurfaceProviders{
    class Vulkan : public SurfaceProvider{
        public:
            inline Vulkan(Window& window) : SurfaceProvider(window){}
            virtual ~Vulkan() = default;

            virtual char const* const* getInstanceExtensions(uint32_t* count) = 0;
            virtual bool createSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) = 0;
            virtual void destroySurface(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* allocator) = 0;

            static std::string_view API() {return "Vulkan";}
    };
}