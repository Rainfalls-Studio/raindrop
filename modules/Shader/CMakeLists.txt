project(Shader
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(EnTT CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

module_link_libraries(
    PUBLIC
        spdlog::spdlog
        glm::glm
        EnTT::EnTT
)

# create library
add_raindrop_module()