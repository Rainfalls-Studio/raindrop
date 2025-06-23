
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Input_test ${SOURCES})
target_link_libraries(
    Raindrop_Input_test PRIVATE
    Raindrop
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Input_test PRIVATE cxx_std_17)

add_test(Input Raindrop_Input_test)