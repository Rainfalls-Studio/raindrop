project(Window
    LANGUAGES CXX
    VERSION 0.1.0 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Window
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

raindrop_module_dependencies(Window
    HARD
        Event
)

find_package(SDL3 REQUIRED)

raindrop_module_link_libraries(Window
    SDL3::SDL3
)