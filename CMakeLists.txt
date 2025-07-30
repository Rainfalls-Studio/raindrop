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

file(GLOB_RECURSE SOURCES "source/*.cpp")

add_library(
    Raindrop STATIC
    ${SOURCES}
)

target_compile_features(Raindrop PUBLIC cxx_std_23)

target_include_directories(
    Raindrop ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(RenderGraph REQUIRED)

find_package(SDL3 REQUIRED) # Not specified in vcpkg.json as it's not currently supported on all linux distros (such as mine)

target_link_libraries(Raindrop
    PRIVATE
    spdlog::spdlog
    glm::glm
    SDL3::SDL3
    Vulkan::Vulkan
    vk-bootstrap::vk-bootstrap
    vk-bootstrap::vk-bootstrap-compiler-warnings
    GPUOpen::VulkanMemoryAllocator
    crg::RenderGraph

    PUBLIC
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