cmake_minimum_required(VERSION 3.14)
project(
    Sim
    VERSION 0.1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(RaindropProject)



file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(Sim ${SOURCES})


raindrop_add_module_dir(Sim "${CMAKE_SOURCE_DIR}/modules")

raindrop_dependencies(Sim
    HARD
        Asset
)


add_raindrop_project(Sim)


add_custom_target(
    sim
    COMMAND $<TARGET_FILE:Sim>
    DEPENDS Sim
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# copy resources into build/

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCE_DIR} ${OUTPUT_RESOURCE_DIR}
    COMMENT "Copying resources to build directory"
)

add_dependencies(sim copy_resources)