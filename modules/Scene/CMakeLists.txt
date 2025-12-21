project(Scene
    LANGUAGES CXX
    VERSION 0.1.0 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Scene
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

find_package(EnTT CONFIG REQUIRED)

raindrop_module_link_libraries(Scene
    EnTT::EnTT
)

raindrop_add_dependencies(Scene
    Scheduler
)
