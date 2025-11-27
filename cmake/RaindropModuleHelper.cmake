
# =============================
#  register_modules(MODULE_DIRS)
# =============================
function(register_modules REGISTRY MODULE_DIRS)
    set(_tmp ${${REGISTRY}})

    foreach(dir IN LISTS MODULE_DIRS)

        # Check if the module directory contains a CMakeLists.txt file
        if (NOT EXISTS "${dir}/CMakeLists.txt")
            message(WARNING "directory ${dir} does not contain a CMakeLists.txt file. Skipped")
            continue()
        endif()

        # setup the module output directory
        # the modules are setup this way : 
        # modules/ (the output binary folder)
        #    MyModule/
        #        bin/
        #        data/
        #        manifest.json
        #    MySecondModule/
        #        ...
        #    MyThirdModule/
        #        ...
        #    ...

        get_filename_component(dir_name ${dir} NAME)
        set(binary_dir "${CMAKE_BINARY_DIR}/modules/${dir_name}")
        set_property(DIRECTORY PROPERTY RAINDROP_MODULE_BINARY_DIR ${binary_dir})

        # Helper functions to setup the module
        include(RaindropModule)
        add_subdirectory(${dir} ${binary_dir} EXCLUDE_FROM_ALL)

        get_property(MODULE_NAME GLOBAL PROPERTY RAINDROP_CURRENT_MODULE_NAME)
        set_property(GLOBAL PROPERTY RAINDROP_CURRENT_MODULE_NAME "")


        # check if module has correctly been built
        if (NOT MODULE_NAME)
            message(WARNING "module \"${MODULE_NAME}\" in \"${dir}\" did not called add_raindrop_module(). Skipped")
            continue()
        endif()

        # If valid name (and thus valid module) store the binary dir
        set_property(GLOBAL PROPERTY RAINDROP_MODULE_${MODULE_NAME}_BINARY_DIR ${binary_dir})
        
        list(APPEND _tmp ${MODULE_NAME})
    endforeach()
        
    set(${REGISTRY} ${_tmp} PARENT_SCOPE)
endfunction()


# =============================
#  discover_modules(MODULES PATH)
# =============================
function(discover_modules MODULES PATH)
    file(GLOB dirs LIST_DIRECTORIES TRUE "${PATH}/*")

    unset(_result)

    foreach(dir IN LISTS dirs)
        if(EXISTS "${dir}/CMakeLists.txt")
            list(APPEND _result ${dir})
        endif()
    endforeach()

    set(${MODULES} "${_result}" PARENT_SCOPE)
endfunction()


# =============================
#  module_exists(NAME)
# =============================
function(module_exists EXISTS NAME)
    get_property(BUILT GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_BUILT)
    set(${EXISTS} ${BUILT} PARENT_SCOPE)
endfunction()



# =============================
#  resolve_dependencies(RESOLVED NAME)
# =============================
function(resolve_dependencies RESOLVED NAME)
    if (${NAME} IN_LIST ${RESOLVED})
        return()
    endif()

    set(_tmp ${${RESOLVED}})
    list(APPEND _tmp ${NAME})
    set(${RESOLVED} ${_tmp} PARENT_SCOPE)

    get_property(HARD GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_HARD_DEPS)
    foreach(dep IN LISTS HARD)
        module_exists(EXISTS ${dep})
        if (EXISTS)
            resolve_dependencies(${RESOLVED} ${dep})
        else()
            message(FATAL_ERROR "Module ${NAME} is missing hard dependency ${dep}")
        endif()
    endforeach()


    get_property(SOFT GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOFT_DEPS)
    foreach(dep IN LISTS SOFT)
        module_exists(EXISTS ${dep})
        if (EXISTS)
            resolve_dependencies(${RESOLVED} ${dep})
        else()
            message(WARNING "Module ${NAME} is missing soft dependency ${dep}")
        endif()
    endforeach()
endfunction()


# =============================
#  create_module_library(NAME)
# =============================
function(create_module_library NAME)
    get_property(LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_LIBRARY)

    if (${LIBRARY})
        # The library has already been created
        return()
    endif()

    # get the source dir
    get_property(SOURCE_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOURCE_DIR)

    if (NOT EXISTS ${SOURCE_DIR})
        message(FATAL_ERROR "The source diectory of module ${NAME} is not defined or valid")
    endif()

    # query files
    file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/src/*")

    # create the library 
    add_library(Module${NAME} SHARED ${SOURCES})

    
    target_include_directories(Module${NAME} PUBLIC "${SOURCE_DIR}/include")
    target_link_libraries(
        Module${NAME} PRIVATE Raindrop::Raindrop
    )

endfunction()