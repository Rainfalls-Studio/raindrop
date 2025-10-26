project(RaindropTestbed LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_testbed ${SOURCES})

target_link_libraries(Raindrop_testbed PRIVATE Raindrop)

add_custom_target(
    testbed
    COMMAND $<TARGET_FILE:Raindrop_testbed>
    DEPENDS Raindrop_testbed
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_folders(Testbed)