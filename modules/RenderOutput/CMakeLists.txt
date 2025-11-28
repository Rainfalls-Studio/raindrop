project(RenderOutput
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

module_link_libraries(
    PUBLIC
        spdlog::spdlog
        Vulkan::Vulkan
        glm::glm
        vk-bootstrap::vk-bootstrap
        vk-bootstrap::vk-bootstrap-compiler-warnings
        GPUOpen::VulkanMemoryAllocator
)

# create library
add_raindrop_module()