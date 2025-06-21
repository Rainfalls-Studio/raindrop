
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Window_test ${SOURCES})
target_link_libraries(
    Raindrop_Window_test PRIVATE
    Raindrop_lib
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Window_test PRIVATE cxx_std_17)

add_test(Window Raindrop_Window_test)