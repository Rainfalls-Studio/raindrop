cmake_minimum_required(VERSION 3.14)


project(
    Sim
    VERSION 0.1.0
    # HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(Sim ${SOURCES})

target_link_libraries(Sim PRIVATE
    Raindrop
)

add_custom_target(
    sim
    COMMAND $<TARGET_FILE:Sim>
    DEPENDS Sim
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
