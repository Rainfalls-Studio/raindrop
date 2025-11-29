project(Shader
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(glslang REQUIRED)
find_package(Vulkan REQUIRED)

module_link_libraries(
    PRIVATE
        glslang::glslang
    PUBLIC
        Vulkan::Vulkan
)

module_dependencies(
    HARD
        Asset
        RenderCore
        Filesystem
)

# create library
add_raindrop_module()