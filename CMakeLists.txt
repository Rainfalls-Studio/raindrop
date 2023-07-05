cmake_minimum_required(VERSION 3.0.0)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/Assimp/" ${CMAKE_PREFIX_PATH})

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_package(tinyobjloader REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${TINYOBJLOADER_INCLUDE_DIRS})

# github submodules
add_subdirectory(libs/glm)
add_subdirectory(libs/gtest)
add_subdirectory(libs/easyloggingpp)
add_subdirectory(libs/tinyxml2)
add_subdirectory(libs/imgui)

# submodules includes
set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/)
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googleTest/include ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googlemock/include)
set(EASY_LOGGING_GPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/easyloggingpp/src)
set(TINY_XML2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2)
set(IMGUI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
set(RAINDROP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/ ${GLM_INCLUDE_DIRS} ${EASY_LOGGING_GPP_INCLUDE_DIRS} ${TINY_XML2_INCLUDE_DIRS}, ${TINY_XML2_INCLUDE_DIRS} ${tinyobjloader_INCLUDE_DIRS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${EASY_LOGGING_GPP_INCLUDE_DIRS})
include_directories(${TINY_XML2_INCLUDE_DIRS})
include_directories(${IMGUI_INCLUDE_DIRS})
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

add_definitions(-DSDL_MAIN_HANDLED)

set(PCH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/Raindrop/common.hpp")

add_subdirectory(src)
add_subdirectory(tests)