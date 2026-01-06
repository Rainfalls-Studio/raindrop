project(CycleManager
    LANGUAGES CXX
    VERSION 0.1.0 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

raindrop_add_module(CycleManager
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)