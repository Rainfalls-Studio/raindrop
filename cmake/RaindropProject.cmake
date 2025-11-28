

# =============================
#   _check_project_not_added()
# =============================
macro(_check_project_not_added TARGET)
    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "_check_project_not_added() received unknown target \"${TARGET}\"")
    endif()

    get_property(BUILT TARGET ${TARGET} PROPERTY RAINDROP_BUILT)

    if (DEFINED BUILT)
        message(FATAL_ERROR "add_raindrop_project() already called")
    endif()
endmacro()


# =============================
#   raindrop_add_module_dir(PATH)
# =============================
function(raindrop_add_module_dir TARGET PATH)
    _check_project_not_added(${TARGET})

    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "raindrop_add_module_dir() received unknown target \"${TARGET}\"")
    endif()

    get_property(existing TARGET ${TARGET} PROPERTY RAINDROP_MODULE_DIRS)

    # Append this module directory
    if(existing)
        set(new_dirs "${existing};${PATH}")
    else()
        set(new_dirs ${PATH})
    endif()

    set_property(TARGET ${TARGET} PROPERTY RAINDROP_MODULE_DIRS ${new_dirs})
endfunction()



# =============================
#  raindrop_dependencies(
#      HARD
#          Module::Filesystem
#          Module::Event
#      SOFT
#          Module::Layer
#  )
# =============================
function(raindrop_dependencies TARGET)
    _check_project_not_added(${TARGET})

    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "raindrop_dependencies() received unknown target \"${TARGET}\"")
    endif()

    set(multiValueArgs HARD SOFT)

    cmake_parse_arguments(DEPS "" "" "${multiValueArgs}" ${ARGN})

    set_property(TARGET ${TARGET} PROPERTY RAINDROP_HARD_DEPS ${DEPS_HARD})
    set_property(TARGET ${TARGET} PROPERTY RAINDROP_SOFT_DEPS ${DEPS_SOFT})
endfunction()


# =============================
#  add_raindrop_project()
# =============================
function(add_raindrop_project TARGET)
    _check_project_not_added(${TARGET})

    message(STATUS "New Raindrop project : ${TARGET}")

    # include internal helper functions
    include(RaindropModuleHelper)

    # list available modules
    get_property(MODULE_DIRS TARGET ${TARGET} PROPERTY RAINDROP_MODULE_DIRS)
    set(MODULES "")

    # Phase 0 - discover modules
    message(STATUS "Discovering module...")
    foreach(dir IN LISTS MODULE_DIRS)
        message(STATUS "\tChecking \"${dir}/\" ...")
        discover_modules(mods ${dir})
        LIST(APPEND MODULES ${mods})
    endforeach()

    
    # Phase 1 - register available modules
    # set(MODULE_REGISTRY)
    register_modules(MODULE_REGISTRY ${MODULES})

    list(LENGTH MODULE_REGISTRY length)

    if (length EQUAL 0)
        message(STATUS "Found 0 module")
    else()
        message(STATUS "Found ${length} module(s) : ")
        foreach (mod IN LISTS MODULE_REGISTRY)
            message("\t- ${mod}")
        endforeach()
    endif()

    # Phase 2 - checking dependencies
    message(STATUS "Checking dependencies...")
    
    get_property(HARD TARGET ${TARGET} PROPERTY RAINDROP_HARD_DEPS)
    get_property(SOFT TARGET ${TARGET} PROPERTY RAINDROP_SOFT_DEPS)

    set(DEPENDENCIES "")
    foreach(dep IN LISTS HARD)
        module_exists(EXISTS ${dep})
        if (EXISTS)
            resolve_dependencies(DEPENDENCIES ${dep})
        else()
            message(FATAL_ERROR "The project ${TARGET} is missing hard dependency ${dep}")
        endif()
    endforeach()

    foreach(dep IN LISTS SOFT)
        module_exists(EXISTS ${dep})
        if (EXISTS)
            resolve_dependencies(DEPENDENCIES ${dep})
        else()
            message(WARNING "The project ${TARGET} is missing soft dependency ${dep}")
        endif()
    endforeach()

    message(STATUS "All required dependencies are present")

    list(LENGTH DEPENDENCIES length)

    if (length EQUAL 0)
        message(STATUS "Using 0 module")
    else()
        message(STATUS "Using ${length} module(s) : ")
        foreach(mod IN LISTS DEPENDENCIES)
            message("\t - ${mod}")
        endforeach()
    endif()

    # Create modules
    message(STATUS "Creating modules...")

    foreach(mod IN LISTS DEPENDENCIES)
        create_module_library(${mod})
    endforeach()
    
    # Link the modules together
    message(STATUS "Linking modules...")

    foreach(mod IN LISTS DEPENDENCIES)
        link_module_library(${mod})
    endforeach()

    # List the project direct dependencies

    set(DIRECT_DEPENDENCIES "")

    foreach(dep IN LISTS HARD)
        module_exists(EXISTS ${dep})

        if (${EXISTS})
            list(APPEND DIRECT_DEPENDENCIES ${dep})
        else()
            message(FATAL_ERROR "The project ${TARGET} is missing hard dependency ${dep}")
        endif()

    endforeach()

    foreach(dep IN LISTS SOFT)
        module_exists(EXISTS ${dep})

        if (${EXISTS})
            list(APPEND DIRECT_DEPENDENCIES ${dep})
        endif()

    endforeach()

    # Link the project to the dependencies

    if (DIRECT_DEPENDENCIES)
        set(MODULES_LIBRARIES "")

        foreach(mod IN LISTS DIRECT_DEPENDENCIES)
            get_property(MOD_LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${mod}_LIBRARY)
            list(APPEND MODULES_LIBRARIES ${MOD_LIBRARY})
        endforeach()

        target_link_libraries(
            ${TARGET}
            PRIVATE
            ${MODULES_LIBRARIES}
        )
    endif()

    set(TARGET_FILE $<TARGET_FILE:${TARGET}>)

    # Directory where the executable is located
    get_target_property(TARGET_DIR ${TARGET} RUNTIME_OUTPUT_DIRECTORY)

    if (NOT TARGET_DIR)
        set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    message("out : ${TARGET_DIR}")

    # Copy the modules
    message(STATUS "Copying modules to the project's modules folder...")



    foreach(mod IN LISTS DEPENDENCIES)
        copy_module_clean(${TARGET} ${mod} "${TARGET_DIR}/modules")
    endforeach()
    

    

    # Link project to Raindrop
    target_link_libraries(${TARGET} PRIVATE Raindrop::Raindrop)

    # confirm project built
    set_property(TARGET ${TARGET} PROPERTY RAINDROP_BUILT TRUE)

endfunction()