project(ImGui
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Dear ImGui module" 
)

find_package(ImGui REQUIRED)
find_package(SDL3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(ImGui
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

raindrop_module_link_libraries(ImGui
    PUBLIC
        imgui::imgui
    
    PRIVATE
        SDL3::SDL3
)

raindrop_add_dependencies(ImGui
    RenderCore
    RenderOutput
    Event
    Window
    Layers
)