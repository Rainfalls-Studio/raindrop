project(RenderCore
    LANGUAGES CXX
    VERSION 0.1.0 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

raindrop_add_module(RenderCore
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

raindrop_add_dependencies(RenderCore
    Window
    Scheduler
)

find_package(vk-bootstrap CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

raindrop_module_link_libraries(RenderCore
    PUBLIC
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
        vk-bootstrap::vk-bootstrap
        vk-bootstrap::vk-bootstrap-compiler-warnings
)

# target_compile_definitions(RenderCore
    # PUBLIC
        # "VULKAN_HPP_NO_EXCEPTIONS"
        # "VULKAN_HPP_ASSERT_ON_RESULT=(expression)((void)0)"
# )
