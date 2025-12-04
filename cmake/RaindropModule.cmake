
macro(__raindrop_check_module_valid MODULE)
    if (NOT TARGET ${MODULE})
        message(FATAL_ERROR "The module ${MODULE} is not a valid module !")
    endif()

    get_target_property(STAMP ${MODULE} RAINDROP_MODULE_STAMP)

    if (NOT STAMP)
        message(FATAL_ERROR "The module ${MODULE} was not created by raindrop_add_module() !")
    endif()
endmacro()



# =============================
#  module_dependencies(
#      HARD
#          Module::Filesystem
#          Module::Event
#      SOFT
#          Module::Layer
#  )
# =============================
function(raindrop_module_dependencies MODULE)
    __raindrop_check_module_valid(${MODULE})

    set(options)
    set(oneValueArgs)
    set(multiValueArgs HARD SOFT)
    cmake_parse_arguments(DEPS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_MODULE_HARD_DEPS "${DEPS_HARD}" PARENT_SCOPE)
    set(_MODULE_SOFT_DEPS "${DEPS_SOFT}" PARENT_SCOPE)

    set_target_properties(${MODULE} PROPERTIES
        RAINDROP_HARD_DEPS "${DEPS_HARD}"
        RAINDROP_SOFT_DEPS "${DEPS_SOFT}"
    )
endfunction()



# =============================
# module_link_libraries(
#     PUBLIC glm::glm
#     PRIVATE spdlog::spdlog
# )
# =============================
function(raindrop_module_link_libraries MODULE)
    __raindrop_check_module_valid(${MODULE})

    get_target_property(INTERFACE ${MODULE} RAINDROP_MODULE_INTERFACE)

    if (NOT INTERFACE)
        message(FATAL_ERROR "[Raindrop] The module ${MODULE} is missing an interface library")
    endif()

    target_link_libraries(${INTERFACE} INTERFACE ${ARGN})
endfunction()



# =============================
#  raindrop_add_module(
#     (REQUIRED) MODULE 
#     (REQUIRED) VERSION 0.0.1
#     (OPTIONAL) DESCRIPTION "Lorem Ipsum" 
#     (REQUIRED) SOURCE sources
#     (OPTIONAL) HEADERS headers 
# )
# =============================
function(raindrop_add_module MODULE)
    set(options)  # No boolean options in your example
    set(oneValueArgs VERSION DESCRIPTION)  # Single value arguments
    set(multiValueArgs SOURCE HEADERS)     # Multi-value arguments
    
    # Parse the arguments passed to the function
    cmake_parse_arguments(
        ARG
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    # ============ Checking arguments

    if(NOT ARG_VERSION)
        message(FATAL_ERROR "[Raindrop] VERSION is required for raindrop_add_module")
    endif()

    if(NOT ARG_SOURCE)
        message(FATAL_ERROR "[Raindrop] SOURCE is required for add_raindrop_module")
    endif()

    message(STATUS "[Raindrop] New module : \"${MODULE}\" (Version : ${ARG_VERSION})")

    add_library(${MODULE} MODULE ${ARG_SOURCE})
    add_library(${MODULE}_interface INTERFACE)

    target_link_libraries(${MODULE} PUBLIC ${MODULE}_interface Raindrop::Engine)

    if(ARG_HEADERS)
        target_include_directories(${MODULE}_interface INTERFACE 
            ${ARG_HEADERS}
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
    endif()

    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/out")
    
    if (WIN32)
        set(LIBRARY_OUTPUT "${OUTPUT_DIR}/bin/windows")
        set(EXPORT_DEFINE "__declspec(dllexport)")
        set_target_properties(${NAME} PROPERTIES SUFFIX ".dll")
    elseif (APPLE)
        set(LIBRARY_OUTPUT "${OUTPUT_DIR}/bin/mac")
        set(EXPORT_DEFINE "__attribute__((visibility(\"default\")))")
        set_target_properties(${NAME} PROPERTIES SUFFIX ".dylib")
    else ()
        set(LIBRARY_OUTPUT "${OUTPUT_DIR}/bin/linux")
        set(EXPORT_DEFINE "__attribute__((visibility(\"default\")))")
        set_target_properties(${NAME} PROPERTIES SUFFIX ".so")
    endif()

    set_target_properties(${MODULE} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT}"
        PREFIX ""
        VERSION "${ARG_VERSION}"
        DESCRIPTION "${ARG_DESCRIPTION}"
        RAINDROP_MODULE_INTERFACE "${MODULE}_interface"
        RAINDROP_MODULE_STAMP TRUE
        RAINDROP_OUTPUT_DIR "${OUTPUT_DIR}"
        RAINDROP_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}"
        RAINDROP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    target_compile_definitions(${MODULE} PRIVATE
        "RAINDROP_EXPORT=${EXPORT_DEFINE}"
        "RAINDROP_CURRENT_MODULE_NAME=\"${MODULE}\""
        "RAINDROP_CURRENT_MODULE_VERSION=\"${ARG_VERSION}\""
    )

    set_target_properties(${MODULE}_interface PROPERTIES
        VERSION ${ARG_VERSION}
        DESCRIPTION "Interface for ${MODULE}"
    )

    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${MODULE} ${MODULE})

    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)
    list(APPEND MODULES ${MODULE})
    set_property(GLOBAL PROPERTY RAINDROP_MODULES ${MODULES})

    set(${MODULE} ${MODULE} PARENT_SCOPE)

endfunction()