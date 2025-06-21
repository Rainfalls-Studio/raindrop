
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_ECS_test ${SOURCES})
target_link_libraries(
    Raindrop_ECS_test PRIVATE
    Raindrop_lib
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_ECS_test PRIVATE cxx_std_17)

add_test(ECS Raindrop_ECS_test)