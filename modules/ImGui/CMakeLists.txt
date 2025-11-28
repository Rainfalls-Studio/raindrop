project(ImGui
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Dear ImGui module" 
)

find_package(spdlog REQUIRED)
find_package(ImGui REQUIRED)

module_link_libraries(
    PUBLIC
        spdlog::spdlog
        imgui::imgui
)

# create library
add_raindrop_module()