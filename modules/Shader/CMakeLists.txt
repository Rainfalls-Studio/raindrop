project(Shader
    LANGUAGES CXX
    VERSION 0.1.0 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Shader
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)


raindrop_add_dependencies(Shader
    Asset
    RenderCore
    Filesystem
)

find_package(glslang REQUIRED)
find_package(Vulkan REQUIRED)

raindrop_module_link_libraries(Shader
    glslang::glslang
    Vulkan::Vulkan
)