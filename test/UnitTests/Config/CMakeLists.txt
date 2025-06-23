
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Config_test ${SOURCES})
target_link_libraries(
    Raindrop_Config_test PRIVATE
    Raindrop
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Config_test PRIVATE cxx_std_17)

add_test(Config Raindrop_Config_test)