project(Window
    LANGUAGES CXX
    VERSION 0.1.0 
)

find_package(SDL3 REQUIRED)

module_link_libraries(
    PUBLIC
        SDL3::SDL3
)

module_dependencies(
    HARD
        Event
)

# create library
add_raindrop_module()