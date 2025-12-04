

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

    message(STATUS "[Raindrop] New project : ${TARGET}")

    # include internal helper functions
    include(RaindropModuleHelper)

    # list available modules
    get_property(AVAILABLE_MODULES GLOBAL PROPERTY RAINDROP_MODULES)
    list(LENGTH AVAILABLE_MODULES AVAILABLE_MODULE_COUNT)
    message(STATUS "[Raindrop] ${AVAILABLE_MODULE_COUNT} module registred...")

    # Phase 2 - checking dependencies
    message(STATUS "[Raindrop] Checking dependencies...")
    
    get_property(HARD TARGET ${TARGET} PROPERTY RAINDROP_HARD_DEPS)
    get_property(SOFT TARGET ${TARGET} PROPERTY RAINDROP_SOFT_DEPS)

    set(DEPENDENCIES "")
    foreach(dep IN LISTS HARD)
        if (TARGET ${dep})
            resolve_dependencies(DEPENDENCIES MODULE_REGISTRY ${dep})
        else()
            message(FATAL_ERROR "[Raindrop] The project ${TARGET} is missing hard dependency ${dep}")
        endif()
    endforeach()

    foreach(dep IN LISTS SOFT)
        module_exists(EXISTS ${dep})
        if (TARGET ${dep})
            resolve_dependencies(DEPENDENCIES MODULE_REGISTRY ${dep})
        else()
            message(WARNING "[Raindrop] The project ${TARGET} is missing soft dependency ${dep}")
        endif()
    endforeach()

    message(STATUS "[Raindrop] All required dependencies are present")

    list(LENGTH DEPENDENCIES length)

    if (length EQUAL 0)
        message(STATUS "[Raindrop] Using 0 module")
    else()
        message(STATUS "[Raindrop] Using ${length} module(s) : ")
        foreach(mod IN LISTS DEPENDENCIES)
            message("\t - ${mod}")
        endforeach()
    endif()

    # List the project direct dependencies

    set(DIRECT_DEPENDENCIES "")
    set(DEFINES "")

    foreach(dep IN LISTS HARD)
        if (TARGET ${dep})
            list(APPEND DIRECT_DEPENDENCIES ${dep})
            list(APPEND DEFINES
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1"
            )
        else()
            message(FATAL_ERROR "The project ${TARGET} is missing hard dependency ${dep}")
        endif()

    endforeach()

    foreach(dep IN LISTS SOFT)
    
        if (TARGET ${dep})
            list(APPEND DIRECT_DEPENDENCIES ${dep})
            list(APPEND DEFINES
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1"
            )
        else()
            list(APPEND DEFINES
                "RAINDROP_MODULE_${dep}_AVAILABLE=0"
            )
        endif()
    endforeach()

    # Link the project to the dependencies

    if (DIRECT_DEPENDENCIES)
        set(MODULES_LIBRARIES "")

        foreach(mod IN LISTS DIRECT_DEPENDENCIES)
            get_target_property(MOD_LIBRARY ${mod} RAINDROP_MODULE_INTERFACE)
            list(APPEND MODULES_LIBRARIES ${MOD_LIBRARY})

            add_dependencies(${TARGET} ${MOD_LIBRARY})
        endforeach()

        if (MODULES_LIBRARIES)
            target_link_libraries(
                ${TARGET}
                PRIVATE
                ${MODULES_LIBRARIES}
            )
        endif()

    endif()

    if (DEFINES)
        target_compile_definitions(
            ${TARGET}
            PRIVATE
            ${DEFINES}
        )
    endif()

    set(TARGET_FILE $<TARGET_FILE:${TARGET}>)

    # Directory where the executable is located
    get_target_property(TARGET_DIR ${TARGET} RUNTIME_OUTPUT_DIRECTORY)

    if (NOT TARGET_DIR)
        set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    # Copy the modules
    message(STATUS "Copying modules to the project's modules folder...")

    foreach(mod IN LISTS DEPENDENCIES)
        copy_module_clean(${TARGET} ${mod} "${TARGET_DIR}/modules")
    endforeach()
    

    # Link project to Raindrop
    target_link_libraries(${TARGET} PRIVATE Raindrop::Engine)

    # Copy the raindrop shader library next to the executable
    get_property(RAINDROP_LIB_PATH GLOBAL PROPERTY RAINDROP_ENGINE_LIBRARY_PATH)

    if (NOT RAINDROP_LIB_PATH)
        message(FATAL_ERROR "The raindrop library path has not been set !")
    endif()

    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${RAINDROP_LIB_PATH} ${TARGET_DIR}
    )

    # confirm project built
    set_property(TARGET ${TARGET} PROPERTY RAINDROP_BUILT TRUE)

    set_target_properties(${TARGET} PROPERTIES
        BUILD_RPATH "\$ORIGIN"
        INSTALL_RPATH "\$ORIGIN"
    )

    message(STATUS "Raindrop project ${TARGET} set up with success !")

endfunction()