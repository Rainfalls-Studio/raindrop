cmake_minimum_required(VERSION 3.20)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# github submodules
add_subdirectory(libs/glm/)
add_subdirectory(libs/gtest/)

# project files
file(GLOB_RECURSE headers_ ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE sources_ ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

# submodules includes
set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} libs/glm/)
set(GTEST_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} libs/gtest/googleTest/include/)

# compile project
include_directories(${CMAKE_CURRENT_SOURCE_DIR} include/ ${GLM_INCLUDE_DIR} ${GTEST_CPP_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC ${headers_} ${sources_})

# tests
file(GLOB_RECURSE test_cources_ ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)
add_executable(${PROJECT_NAME}Test ${test_cources_})
target_link_libraries(${PROJECT_NAME}Test gtest_main gtest Raindrop)