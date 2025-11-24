
# =============================
#  Module global context
# =============================

set(RAINDROP_MODULES "" CACHE INTERNAL "Registered engine modules")

set(_MODULE_NAME "")
set(_MODULE_VERSION "0.0.0")

set(_MODULE_HARD_DEPS "")
set(_MODULE_SOFT_DEPS "")

set(_MODULE_LINK_PUBLIC_LIBS "")
set(_MODULE_LINK_PRIVATE_LIBS "")
set(_MODULE_LINK_INTERFACE_LIBS "")


# =============================
#   module_name(NAME)
# =============================
function(module_name NAME)
    if (DEFINED _MODULE_HAS_BEEN_BUILT)
        message(FATAL_ERROR "add_raindrop_module() already called")
    endif()

    set(_MODULE_NAME "${NAME}" PARENT_SCOPE)
endfunction()


# =============================
#   module_version("1.0.0")
# =============================
function(module_version VERSION)
    if (DEFINED _MODULE_HAS_BEEN_BUILT)
        message(FATAL_ERROR "add_raindrop_module() already called")
    endif()

    set(_MODULE_VERSION "${VERSION}" PARENT_SCOPE)
endfunction()


# =============================
#  module_dependencies(...)
# =============================
function(module_dependencies)
    if (DEFINED _MODULE_HAS_BEEN_BUILT)
        message(FATAL_ERROR "add_raindrop_module() already called")
    endif()

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
    if (DEFINED _MODULE_HAS_BEEN_BUILT)
        message(FATAL_ERROR "add_raindrop_module() already called")
    endif()

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
    if (DEFINED _MODULE_HAS_BEEN_BUILT)
        message(FATAL_ERROR "add_raindrop_module() called twice")
    endif()
    set(_MODULE_HAS_BEEN_BUILT TRUE)

    # check if name is setup
    if(NOT _MODULE_NAME)
        message(FATAL_ERROR "module_name() must be called before add_raindrop_module()")
    endif()

    # create a cmake struct
    set(MOD "NAME=${_MODULE_NAME}")
    set(MOD "${MOD}&VERSION=${_MODULE_VERSION}")
    set(MOD "${MOD}&HARD_DEPS=${_MODULE_HARD_DEPS}")
    set(MOD "${MOD}&SOFT_DEPS=${_MODULE_SOFT_DEPS}")
    set(MOD "${MOD}&PUBLIC_LIBS=${_MODULE_LINK_PUBLIC_LIBS}")
    set(MOD "${MOD}&PRIVATE_LIBS=${_MODULE_LINK_PRIVATE_LIBS}")
    set(MOD "${MOD}&INTERFACE_LIBS=${_MODULE_LINK_INTERFACE_LIBS}")
    set(MOD "${MOD}&SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

    list(APPEND RAINDROP_MODULES "${MOD}")
    set(RAINDROP_MODULES "${RAINDROP_MODULES}" CACHE INTERNAL "Registered engine modules")
endfunction()