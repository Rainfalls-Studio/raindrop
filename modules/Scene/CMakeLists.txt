project(Scene
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(EnTT CONFIG REQUIRED)

module_link_libraries(
    PUBLIC
        EnTT::EnTT
)

# create library
add_raindrop_module()