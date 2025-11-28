project(Window
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm CONFIG REQUIRED)

module_link_libraries(
    PUBLIC
        spdlog::spdlog
        SDL3::SDL3
        glm::glm
)

# create library
add_raindrop_module()