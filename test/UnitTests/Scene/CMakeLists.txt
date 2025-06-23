
file(GLOB_RECURSE SOURCES "./*.cpp")

add_executable(Raindrop_Scene_test ${SOURCES})
target_link_libraries(
    Raindrop_Scene_test PRIVATE
    Raindrop
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
target_compile_features(Raindrop_Scene_test PRIVATE cxx_std_17)

add_test(Scene Raindrop_Scene_test)