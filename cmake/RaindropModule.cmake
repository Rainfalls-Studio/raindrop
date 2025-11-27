
# =============================
#  Module global context
# =============================

set(_MODULE_NAME ${CMAKE_PROJECT_NAME})
set(_MODULE_VERSION ${CMAKE_PROJECT_VERSION})

set(_MODULE_HARD_DEPS "")
set(_MODULE_SOFT_DEPS "")

set(_MODULE_LINK_PUBLIC_LIBS "")
set(_MODULE_LINK_PRIVATE_LIBS "")
set(_MODULE_LINK_INTERFACE_LIBS "")


macro(_check_module_not_built)
    get_property(BUILT GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_BUILT)
    if (BUILT)
        message(FATAL_ERROR "add_raindrop_module() already called")
    endif()
endmacro()


# =============================
#   module_name(NAME)
# =============================
function(module_name NAME)
    _check_module_not_built()

    set(_MODULE_NAME "${NAME}" PARENT_SCOPE)
endfunction()


# =============================
#   module_version("1.0.0")
# =============================
function(module_version VERSION)
    _check_module_not_built()

    set(_MODULE_VERSION "${VERSION}" PARENT_SCOPE)
endfunction()


# =============================
#  module_dependencies(
#      HARD
#          Module::Filesystem
#          Module::Event
#      SOFT
#          Module::Layer
#  )
# =============================
function(module_dependencies)
    _check_module_not_built()

    set(options)
    set(oneValueArgs)
    set(multiValueArgs HARD SOFT)
    cmake_parse_arguments(DEPS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_MODULE_HARD_DEPS "${DEPS_HARD}" PARENT_SCOPE)
    set(_MODULE_SOFT_DEPS "${DEPS_SOFT}" PARENT_SCOPE)
endfunction()


# =============================
# module_link_libraries(
#     PUBLIC glm::glm
#     PRIVATE spdlog::spdlog
# )
# =============================
function(module_link_libraries)
    _check_module_not_built()

    set(current_visibility "")

    foreach(arg IN LISTS ARGN)
        if(arg STREQUAL "PUBLIC" OR arg STREQUAL "PRIVATE" OR arg STREQUAL "INTERFACE")
            # We switched to a new block
            set(current_visibility "${arg}")
        else()
            # It's a library → store in the correct group
            if(current_visibility STREQUAL "PUBLIC")
                list(APPEND _MODULE_LINK_PUBLIC_LIBS "${arg}")
            elseif(current_visibility STREQUAL "PRIVATE")
                list(APPEND _MODULE_LINK_PRIVATE_LIBS "${arg}")
            elseif(current_visibility STREQUAL "INTERFACE")
                list(APPEND _MODULE_LINK_INTERFACE_LIBS "${arg}")
            else()
                message(FATAL_ERROR "module_link_libraries(): library '${arg}' provided outside PUBLIC/PRIVATE/INTERFACE block.")
            endif()
        endif()
    endforeach()

    # Push values to parent scope (so add_raindrop_module can use them)
    set(_MODULE_LINK_PUBLIC_LIBS "${_MODULE_LINK_PUBLIC_LIBS}" PARENT_SCOPE)
    set(_MODULE_LINK_PRIVATE_LIBS "${_MODULE_LINK_PRIVATE_LIBS}" PARENT_SCOPE)
    set(_MODULE_LINK_INTERFACE_LIBS "${_MODULE_LINK_INTERFACE_LIBS}" PARENT_SCOPE)
endfunction()

# =============================
#  add_raindrop_module()
# =============================
function(add_raindrop_module)
    _check_module_not_built()

    # check if name is setup
    if(NOT _MODULE_NAME)
        message(FATAL_ERROR "module_name() must be called before add_raindrop_module()")
    endif()

    set_property(GLOBAL PROPERTY RAINDROP_CURRENT_MODULE_NAME ${_MODULE_NAME})

    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_BUILT TRUE)
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_VERSION ${_MODULE_VERSION})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_HARD_DEPS ${_MODULE_HARD_DEPS})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_SOFT_DEPS ${_MODULE_SOFT_DEPS})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_LINK_PUBLIC_LIBS ${_MODULE_LINK_PUBLIC_LIBS})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_LINK_PRIVATE_LIBS ${_MODULE_LINK_PRIVATE_LIBS})
    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${_MODULE_NAME}_LINK_INTERFACE_LIBS ${_MODULE_LINK_INTERFACE_LIBS})

    set(_MODULE_HAS_BEEN_BUILT TRUE PARENT_SCOPE)
endfunction()