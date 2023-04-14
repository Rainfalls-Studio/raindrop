cmake_minimum_required(VERSION 3.20)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# github submodules
add_subdirectory(libs/glm)
add_subdirectory(libs/gtest)

# submodules includes
set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/)
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googleTest/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googlemock/include)

set(RAINDROP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/ ${GLM_INCLUDE_DIRS}")
set(CURRENT_SCOPE ${CMAKE_PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)