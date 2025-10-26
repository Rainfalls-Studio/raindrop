include(cmake/folders.cmake)

target_compile_definitions(Raindrop PRIVATE SPDLOG_ACTIVE_LEVEL=0)

include(CTest)
if(BUILD_TESTING)
  target_compile_definitions(Raindrop PRIVATE RAINDROP_MOCK)
  # add_subdirectory(test)
endif()

option(BUILD_TESTBED "Build the testbed application" ON)
if(BUILD_TESTBED)
  add_subdirectory(testbed)
endif()

option(BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
if(BUILD_MCSS_DOCS)
  include(cmake/docs.cmake)
endif()

option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(ENABLE_COVERAGE)
  include(cmake/coverage.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)