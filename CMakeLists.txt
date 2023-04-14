cmake_minimum_required(VERSION 3.20)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# github submodules
add_subdirectory(libs/glm)
add_subdirectory(libs/gtest)

# submodules includes
set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/glm/)
set(GTEST_INCLUDE_DIRS libs/gtest/googleTest/include libs/gtest/googlemock/include)

set(CURRENT_SCOPE ${CMAKE_PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GLM_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)

# # project files
# file(GLOB_RECURSE headers_ ${CMAKE_CURRENT_SOURCE_DIR} include/*.hpp)
# file(GLOB_RECURSE sources_ ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)


# set(RAINDROP_INCLUDE_DIRS ${GLM_INCLUDE_DIR} include/)

# # compile project
# include_directories(${CMAKE_CURRENT_SOURCE_DIR} include/ ${GLM_INCLUDE_DIR} ${GTEST_CPP_INCLUDE_DIR})
# add_library(${PROJECT_NAME} STATIC ${headers_} ${sources_})

# # tests
# file(GLOB_RECURSE test_cources_ ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)
# add_executable(${PROJECT_NAME}Test ${test_cources_})
# target_link_libraries(${PROJECT_NAME}Test gtest_main gtest Raindrop)