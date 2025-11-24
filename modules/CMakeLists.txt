
# ============================================
#  Raindrop Engine - Module Loader (3-phase)
# ============================================

message(STATUS "== Raindrop: Discovering modules ==")

# --------------------------------------------
#  Phase 0: Prepare registry
# --------------------------------------------
# This variable is populated by add_raindrop_module()
# inside each module's own CMakeLists.txt.
set(RAINDROP_MODULES "" CACHE INTERNAL "Registered engine modules")

# Path to the module DSL (AddModule.cmake)
set(CMAKE_PROJECT_INCLUDE_BEFORE
    "${CMAKE_SOURCE_DIR}/cmake/AddModule.cmake"
)


# --------------------------------------------
#  Phase 1: Discover modules and load metadata
# --------------------------------------------
file(GLOB MODULE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(MDIR ${MODULE_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${MDIR}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${MDIR}/CMakeLists.txt")
            message(STATUS "  - Loading module: ${MDIR}")

            # Load metadata only (module_name, deps, etc.)
            # EXCLUDE_FROM_ALL prevents accidental build.
            add_subdirectory(${MDIR} EXCLUDE_FROM_ALL)
        endif()
    endif()
endforeach()

message(STATUS "== Raindrop: Metadata loaded for modules ==")

# --------------------------------------------
#  Helper function: Create a module
# --------------------------------------------
function(create_module MOD_STRUCT)
    # Extract fields
    string(REGEX MATCH "NAME=([^&]+)" _match "${MOD_STRUCT}")
    set(NAME "${CMAKE_MATCH_1}")
    set(MOD_NAME "Module${NAME}")

    string(REGEX MATCH "VERSION=([^&]+)" _match "${MOD_STRUCT}")
    set(VERSION "${CMAKE_MATCH_1}")

    string(REGEX MATCH "SOURCE_DIR=([^&]+)" _match "${MOD_STRUCT}")
    set(SOURCE_DIR "${CMAKE_MATCH_1}")

    message(STATUS "Creating module ${NAME}")

    # Collect sources
    file(GLOB_RECURSE MODULE_SOURCES "${SOURCE_DIR}/src/*.cpp")


    # Real library
    add_library(${MOD_NAME} SHARED ${MODULE_SOURCES})

    # Module include folder
    target_include_directories(${MOD_NAME} PUBLIC "${SOURCE_DIR}/include")

endfunction()

# --------------------------------------------
#  Phase 2: Create modules
# --------------------------------------------

message(STATUS "== Raindrop: Create modules ==")

foreach(MOD_STRUCT ${RAINDROP_MODULES})
    create_module("${MOD_STRUCT}")
endforeach()



# --------------------------------------------
#  Helper function: Create a module
# --------------------------------------------
function(link_module MOD_STRUCT)
    # Extract fields
    string(REGEX MATCH "NAME=([^&]+)" _match "${MOD_STRUCT}")
    set(NAME "${CMAKE_MATCH_1}")
    set(MOD_NAME "Module${NAME}")

    string(REGEX MATCH "HARD_DEPS=([^&]*)" _match "${MOD_STRUCT}")
    set(HARD_DEPS "${CMAKE_MATCH_1}")

    string(REGEX MATCH "SOFT_DEPS=([^&]*)" _match "${MOD_STRUCT}")
    set(SOFT_DEPS "${CMAKE_MATCH_1}")

    string(REGEX MATCH "PUBLIC_LIBS=([^&]*)" _match "${MOD_STRUCT}")
    set(MOD_PUBLIC_LIBS "${CMAKE_MATCH_1}")

    string(REGEX MATCH "PRIVATE_LIBS=([^&]*)" _match "${MOD_STRUCT}")
    set(MOD_PRIVATE_LIBS "${CMAKE_MATCH_1}")

    string(REGEX MATCH "INTERFACE_LIBS=([^&]*)" _match "${MOD_STRUCT}")
    set(MOD_INTERFACE_LIBS "${CMAKE_MATCH_1}")

    message(STATUS "Linking module ${NAME}")

    # Link engine core
    target_link_libraries(${MOD_NAME} PUBLIC Raindrop::Raindrop)

    # HARD dependencies -> PUBLIC
    foreach(dep ${HARD_DEPS})
        if(NOT TARGET Modules::${dep})
            message(FATAL_ERROR "Module ${NAME} requires missing HARD dependency: ${dep}")
        endif()
        target_link_libraries(${MOD_NAME} PUBLIC Modules::${dep})
    endforeach()

    # SOFT dependencies -> PRIVATE (only if exist)
    foreach(dep ${SOFT_DEPS})
        if(TARGET Modules::${dep})
            target_link_libraries(${MOD_NAME} PRIVATE Modules::${dep})
        else()
            message(STATUS "  Soft dependency ${dep} not found → ignored")
        endif()
    endforeach()

    # Extra user-specified libraries
    if(MOD_PUBLIC_LIBS)
        target_link_libraries(${MOD_NAME} PUBLIC ${MOD_PUBLIC_LIBS})
    endif()

    if(MOD_PRIVATE_LIBS)
        target_link_libraries(${MOD_NAME} PRIVATE ${MOD_PRIVATE_LIBS})
    endif()

    if(MOD_INTERFACE_LIBS)
        target_link_libraries(${MOD_NAME} INTERFACE ${MOD_INTERFACE_LIBS})
    endif()
endfunction()


# --------------------------------------------
#  Phase 3: Link modules
# --------------------------------------------

message(STATUS "== Raindrop: Link modules ==")

foreach(MOD_STRUCT ${RAINDROP_MODULES})
    link_module("${MOD_STRUCT}")
endforeach()


message(STATUS "== Raindrop: All modules built successfully ==")
