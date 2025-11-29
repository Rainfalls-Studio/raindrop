
# =============================
#  register_modules(REGISTRY MODULE_DIRS)
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

    set(${MODULES} ${_result} PARENT_SCOPE)
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
function(resolve_dependencies RESOLVED REGISTRY NAME)
    if (NAME IN_LIST ${RESOLVED})
        return()
    endif()

    list(APPEND ${RESOLVED} "${NAME}")

    # Get hard dependencies
    get_property(HARD GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_HARD_DEPS)

    foreach(dep IN LISTS HARD)
        if (dep IN_LIST ${REGISTRY})
            resolve_dependencies(${RESOLVED} ${REGISTRY} "${dep}")
        else()
            message(FATAL_ERROR "Module ${NAME} is missing hard dependency ${dep}")
        endif()
    endforeach()


    get_property(SOFT GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOFT_DEPS)
    foreach(dep IN LISTS SOFT)
        if (dep IN_LIST ${REGISTRY})
            resolve_dependencies(${RESOLVED} "${REGISTRY}" "${dep}")
        else()
            message(WARNING "Module ${NAME} is missing soft dependency ${dep}")
        endif()
    endforeach()


    set(${RESOLVED} "${${RESOLVED}}" PARENT_SCOPE)
endfunction()



# =============================
#  create_module_manifest(NAME)
# =============================
function(create_module_manifest NAME)
    get_property(VERSION GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_VERSION)
    get_property(DESCRIPTION GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_DESCRIPTION)
    get_property(HARD_DEPS GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_HARD_DEPS)
    get_property(SOFT_DEPS GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOFT_DEPS)
    get_property(BINARY_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_BINARY_DIR)

    # === Validation
    if (NOT VERSION)
        message(FATAL_ERROR "Module ${NAME} is missing a version")
    endif()

    # === Format dependencies
    # == HARD
    set(HARD_JSON "")

    foreach (dep IN LISTS HARD_DEPS)
        list(GET dep 0 dep_name)
        string(APPEND HARD_JSON "\t\t\t\"${dep_name}\": \"0.0.0\",\n")
    endforeach()

    if (HARD_JSON)
        # remove trailing comma
        string(REGEX REPLACE ",\n$" "\n" HARD_JSON "${HARD_JSON}") 
    endif()

    # == SOFT
    set(SOFT_JSON "")

    foreach (dep IN LISTS SOFT_DEPS)
        list(GET dep 0 dep_name)
        string(APPEND SOFT_JSON "\t\t\t\"${dep_name}\": \"0.0.0\",\n")
    endforeach()

    if (SOFT_JSON)
        # remove trailing comma
        string(REGEX REPLACE ",\n$" "\n" SOFT_JSON "${SOFT_JSON}") 
    endif()

    # === variable setup for manifest template

    set(MODULE_NAME ${NAME})
    set(MODULE_VERSION ${VERSION})
    set(MODULE_DESCRIPTION ${DESCRIPTION})
    set(MODULE_HARD_DEPENDENCIES ${HARD_JSON})
    set(MODULE_SOFT_DEPENDENCIES ${SOFT_JSON})

    set(MANIFEST_OUTPUT "${BINARY_DIR}/clean/manifest.json")

    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/moduleManifest.json.in"
        ${MANIFEST_OUTPUT}
        @ONLY
    )
endfunction()


# =============================
#  copy_module_data(NAME)
# =============================
function(copy_module_data NAME)
    get_property(SOURCE_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOURCE_DIR)
    get_property(BINARY_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_BINARY_DIR)

    if (NOT SOURCE_DIR)
        message(FATAL_ERROR "The source directory of module \"${NAME}\" has not been setup correctly")
    endif()

    if (NOT BINARY_DIR)
        message(FATAL_ERROR "The binary directory of module \"${NAME}\" has not been setup correctly")
    endif()

    
    set(SRC "${SOURCE_DIR}/data")
    set(DST "${BINARY_DIR}/clean/data")

    if (NOT EXISTS ${SRC})
        return()
    endif()

    file(GLOB_RECURSE SRC_FILES "${SRC}/*")

    add_custom_command(
        OUTPUT "${BINARY_DIR}/data.stamp"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC} ${DST}
        COMMAND ${CMAKE_COMMAND} -E touch "${BINARY_DIR}/data.stamp"
        DEPENDS ${SRC_FILES}
        COMMENT "Copying ${SRC} to ${DST}"
    )

    add_custom_target(copy_${NAME}_data_folder ALL DEPENDS "${BINARY_DIR}/data.stamp")
endfunction()



# =============================
#  create_module_library(NAME)
# =============================
function(create_module_library NAME)
    get_property(LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_LIBRARY)

    if (LIBRARY)
        # The library has already been created
        return()
    endif()

    # get the source dir
    get_property(SOURCE_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOURCE_DIR)
    get_property(BIN_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_BINARY_DIR)

    if (NOT EXISTS ${SOURCE_DIR})
        message(FATAL_ERROR "The source diectory of module ${NAME} is not defined or valid")
    endif()

    # query files
    file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/src/*")

    # create the library 
    add_library(${NAME} SHARED ${SOURCES})

    # Set the library output
    set(LIBRARY_OUTPUT "")
    if (WIN32)
        set(LIBRARY_OUTPUT "${BIN_DIR}/clean/bin/windows")
    elseif (APPLE)
        set(LIBRARY_OUTPUT "${BIN_DIR}/clean/bin/mac")
    else ()
        set(LIBRARY_OUTPUT "${BIN_DIR}/clean/bin/linux")
    endif()

    set_target_properties(${NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT}
        RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT}
    )

    # include directories and link raindrop

    target_include_directories(${NAME} PUBLIC "${SOURCE_DIR}/include")
    target_link_libraries(
        ${NAME} PRIVATE Raindrop::Raindrop
    )

    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_LIBRARY ${NAME})
    create_module_manifest(${NAME})
    copy_module_data(${NAME})

endfunction()


# =============================
#  link_module_library(NAME)
# =============================
function(link_module_library NAME)
    get_property(LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_LIBRARY)

    if (NOT LIBRARY)
        message(FATAL_ERROR "Cannot link module ${NAME}, it's library has not been created")
    endif()

    set(DEPENDENCIES "")
    set(DEFINES "")

    get_property(HARD GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_HARD_DEPS)
    foreach(dep IN LISTS HARD)
        get_property(DEP_LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${dep}_LIBRARY)

        if (DEP_LIBRARY)
            list(APPEND DEPENDENCIES ${DEP_LIBRARY})
            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1" # TODO: support versioning
            )
        else()
            message(FATAL_ERROR "Module ${NAME} is missing hard dependency ${dep}")
        endif()
    endforeach()


    get_property(SOFT GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOFT_DEPS)

    foreach(dep IN LISTS SOFT)
        get_property(DEP_LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${dep}_LIBRARY)
        
        if (DEP_LIBRARY)
            list(APPEND DEPENDENCIES ${DEP_LIBRARY})

            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1" # TODO: support versioning
            )
        else()
            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=0"
            )
        endif()
    endforeach()

    if (DEPENDENCIES)
        target_link_libraries(
            ${LIBRARY}
            PUBLIC
            ${DEPENDENCIES}
        )
    endif()

    if (DEFINES)
        target_compile_definitions(
            ${LIBRARY}
            PUBLIC ${DEFINES}
        )
    endif()

    target_compile_definitions(
        ${LIBRARY}
        PRIVATE
            "RAINDROP_CURRENT_MODULE_NAME=\"${NAME}\""
            "RAINDROP_CURRENT_MODULE_VERSION=1"
    )

endfunction()



# =============================
#  copy_module_clean(TARGET NAME DESTINATION)
# =============================
function(copy_module_clean TARGET NAME DESTINATION)
    get_property(BINARY_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_BINARY_DIR)
    get_property(SOURCE_DIR GLOBAL PROPERTY RAINDROP_MODULE_${NAME}_SOURCE_DIR)

    if (NOT BINARY_DIR)
        message(FATAL_ERROR "The module \"${NAME}\" doesn't have a valid binary directory")
    endif()

    get_filename_component(DIR_NAME ${SOURCE_DIR} NAME)

    set(CLEAN_ROOT "${BINARY_DIR}/clean")
    set(MODULE_DST "${DESTINATION}/${DIR_NAME}")

    set(STAMP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.stamp")

    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E make_directory "${MODULE_DST}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CLEAN_ROOT} ${MODULE_DST}
        COMMENT "[Module] Cleaning and copying module ${NAME} to ${MODULE_DST}"
    )

endfunction()