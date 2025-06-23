
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Network_test ${SOURCES})
target_link_libraries(
    Raindrop_Network_test PRIVATE
    Raindrop
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Network_test PRIVATE cxx_std_17)

add_test(Network Raindrop_Network_test)