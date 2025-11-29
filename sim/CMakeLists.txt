cmake_minimum_required(VERSION 3.14)
project(
    Sim
    VERSION 0.1.0
    LANGUAGES CXX
)

# include raindrop
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(RaindropProject)


# create executable

file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(Sim ${SOURCES})

set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/Sim/")

set_target_properties(Sim PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/Sim/")


# add raindrop modules
raindrop_add_module_dir(Sim "${CMAKE_SOURCE_DIR}/modules")

raindrop_dependencies(Sim
    HARD
        Asset
        Event
        Window
)


add_raindrop_project(Sim)


# =============================
# === Copy Resources
# =============================

add_custom_target(
    sim
    COMMAND $<TARGET_FILE:Sim>
    DEPENDS Sim
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# copy resources into build/

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCE_DIR "${OUTPUT_DIRECTORY}/resources")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCE_DIR} ${OUTPUT_RESOURCE_DIR}
    COMMENT "Copying resources to build directory"
)

add_dependencies(sim copy_resources)