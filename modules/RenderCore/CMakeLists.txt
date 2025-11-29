project(RenderCore
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(vk-bootstrap CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

module_link_libraries(
    PUBLIC
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
        vk-bootstrap::vk-bootstrap
        vk-bootstrap::vk-bootstrap-compiler-warnings
)

module_dependencies(
    HARD
        Window
)

# create library
add_raindrop_module()