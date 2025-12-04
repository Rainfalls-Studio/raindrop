project(Filesystem
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "A filesystem abstraction layer to ease virtual storage and mocking" 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Filesystem
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)