cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    Raindrop
    VERSION 0.1.0
    DESCRIPTION "Game engine"
    # HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(
    Raindrop_lib STATIC
    ${SOURCES}
)

target_include_directories(
    Raindrop_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

target_compile_features(Raindrop_lib PUBLIC cxx_std_17)

find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

target_link_libraries(Raindrop_lib
    PRIVATE
    spdlog::spdlog
    EnTT::EnTT
)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT Raindrop_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of Raindrop"
  )
endif()

include(cmake/dev-mode.cmake)