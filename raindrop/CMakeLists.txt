cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    Raindrop
    VERSION 0.1.0
    DESCRIPTION "Game engine"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(RaindropEngine SHARED ${SOURCES})

target_compile_features(RaindropEngine PUBLIC cxx_std_23)

target_include_directories(
    RaindropEngine ${warning_guard}
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

find_package(SDL3 REQUIRED) # Not specified in vcpkg.json as it's not currently supported on all linux distros (such as mine)

target_link_libraries(RaindropEngine
    PUBLIC
        spdlog::spdlog
        glm::glm
    
    PRIVATE
        nlohmann_json::nlohmann_json
)

add_library(Raindrop::Engine ALIAS RaindropEngine)

target_compile_definitions(RaindropEngine
    PUBLIC
        "VULKAN_HPP_NO_EXCEPTIONS"
        # "VULKAN_HPP_ASSERT_ON_RESULT=(expression)((void)0)"
)

add_custom_command(TARGET RaindropEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
        "Setting RAINDROP_ENGINE_LIBRARY_PATH to: $<TARGET_FILE:RaindropEngine>"
)

set_property(GLOBAL PROPERTY RAINDROP_ENGINE_LIBRARY_PATH "$<TARGET_FILE:RaindropEngine>")

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT Raindrop_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  # message(
  #     AUTHOR_WARNING
  #     "Developer mode is intended for developers of Raindrop"
  # )
endif()

include(cmake/dev-mode.cmake)