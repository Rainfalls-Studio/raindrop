project(Asset
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Simple asset manager with factories and async friendly loading" 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Asset
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

raindrop_module_dependencies(Asset
    HARD
        Filesystem
)