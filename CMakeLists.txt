cmake_minimum_required(VERSION 3.14)
project(Raindrop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(modules)
add_subdirectory(raindrop)
add_subdirectory(sim)