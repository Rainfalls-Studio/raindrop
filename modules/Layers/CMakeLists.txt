project(Layers
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Layer managment module for handling data transmission" 
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*")

raindrop_add_module(Layers
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    SOURCE ${SOURCES}
    HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)