
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Event_test ${SOURCES})
target_link_libraries(
    Raindrop_Event_test PRIVATE
    Raindrop_lib
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Event_test PRIVATE cxx_std_17)

add_test(Event Raindrop_Event_test)