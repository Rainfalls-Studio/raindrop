project(Layers
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Layer managment module for handling data transmission" 
)

find_package(spdlog REQUIRED)

module_link_libraries(
    PUBLIC spdlog::spdlog
)

# create library
add_raindrop_module()