project(ImGui
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Dear ImGui module" 
)

find_package(ImGui REQUIRED)
find_package(SDL3 REQUIRED)

module_link_libraries(
    PUBLIC
        imgui::imgui
        SDL3::SDL3
)

module_dependencies(
    HARD
        RenderCore
        RenderOutput
        Event
        Window
        Layers
)

# create library
add_raindrop_module()