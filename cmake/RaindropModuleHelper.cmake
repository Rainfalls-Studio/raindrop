
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
function(module_exists MODULE NAME)
    get_property(MODULE GLOBAL PROPERTY RAINDROP_MODULE_${NAME})
    set(${MODULE} ${MODULE} PARENT_SCOPE)
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
function(create_module_manifest MODULE)
    get_target_property(VERSION ${MODULE} VERSION)
    get_target_property(DESCRIPTION ${MODULE} DESCRIPTION)
    get_target_property(HARD_DEPS ${MODULE} RAINDROP_HARD_DEPS)
    get_target_property(SOFT_DEPS ${MODULE} RAINDROP_SOFT_DEPS)
    get_target_property(OUTPUT_DIR ${MODULE} RAINDROP_OUTPUT_DIR)

    # === Validation
    if (NOT VERSION)
        message(FATAL_ERROR "Module ${MODULE} is missing a version")
    endif()

    if (NOT DESCRIPTION)
        set(DESCRIPTION "")
    endif()

    if (NOT HARD_DEPS)
        set(HARD_DEPS "")
    endif()

    if (NOT SOFT_DEPS)
        set(SOFT_DEPS "")
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

    # === the binaries
    
    set(MODULE_WINDOWS_BIN_PATH)
    set(MODULE_LINUX_BIN_PATH)
    set(MODULE_MAC_BIN_PATH)

    if (WIN32)
        set(MODULE_WINDOWS_BIN_PATH "bin/windows/${MODULE}.dll")
    elseif (APPLE)
        set(MODULE_LINUX_BIN_PATH "bin/max/${MODULE}.dylib")
    else ()
        set(MODULE_LINUX_BIN_PATH "bin/linux/${MODULE}.so")
    endif()

    # === variable setup for manifest template

    set(MODULE_NAME ${MODULE})
    set(MODULE_VERSION ${VERSION})
    set(MODULE_DESCRIPTION ${DESCRIPTION})
    set(MODULE_HARD_DEPENDENCIES ${HARD_JSON})
    set(MODULE_SOFT_DEPENDENCIES ${SOFT_JSON})


    set(MANIFEST_OUTPUT "${OUTPUT_DIR}/manifest.json")

    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/moduleManifest.json.in"
        ${MANIFEST_OUTPUT}
        @ONLY
    )
endfunction()


# =============================
#  copy_module_data(MODULE)
# =============================
function(copy_module_data MODULE)
    get_target_property(OUTPUT_DIR ${MODULE} RAINDROP_OUTPUT_DIR)
    get_target_property(SOURCE_DIR ${MODULE} RAINDROP_SOURCE_DIR)
    get_target_property(BINARY_DIR ${MODULE} RAINDROP_BINARY_DIR)

    if (NOT SOURCE_DIR)
        message(FATAL_ERROR "[Raindrop] The source directory of module \"${MODULE}\" has not been setup correctly")
    endif()

    if (NOT OUTPUT_DIR)
        message(FATAL_ERROR "[Raindrop] The output directory of module \"${MODULE}\" has not been setup correctly")
    endif()

    if (NOT BINARY_DIR)
        message(FATAL_ERROR "[Raindrop] The binary directory of module \"${MODULE}\" has not been setup correctly")
    endif()

    
    set(SRC "${SOURCE_DIR}/data")
    set(DST "${OUTPUT_DIR}/data")

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
#  link_module_library(NAME)
# =============================
function(link_module_library MODULE)
    set(DEPENDENCIES "")
    set(DEFINES "")

    get_property(HARD GLOBAL PROPERTY RAINDROP_MODULE_${MODULE}_HARD_DEPS)
    foreach(dep IN LISTS HARD)
        get_target_property(interface_library ${dep} RAINDROP_MODULE_INTERFACE)

        if (DEP_LIBRARY)
            list(APPEND DEPENDENCIES ${DEP_LIBRARY})
            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1" # TODO: support versioning
            )

            add_dependencies(${MODULE} ${DEP_LIBRARY})
        else()
            message(FATAL_ERROR "Module ${NAME} is missing hard dependency ${dep}")
        endif()
    endforeach()


    get_property(SOFT GLOBAL PROPERTY RAINDROP_MODULE_${MODULE}_SOFT_DEPS)

    foreach(dep IN LISTS SOFT)
        get_property(DEP_LIBRARY GLOBAL PROPERTY RAINDROP_MODULE_${dep}_INTERFACE)
        
        if (DEP_LIBRARY)
            list(APPEND DEPENDENCIES ${DEP_LIBRARY})

            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=1"
                "RAINDROP_MODULE_${dep}_VERSION=1" # TODO: support versioning
            )

            add_dependencies(${MODULE} ${DEP_LIBRARY})
        else()
            list(APPEND DEFINES 
                "RAINDROP_MODULE_${dep}_AVAILABLE=0"
            )
        endif()
    endforeach()

    if (DEPENDENCIES)
        target_link_libraries(${MODULE} PUBLIC ${DEPENDENCIES})
    endif()

    if (DEFINES)
        target_compile_definitions(${MODULE} PUBLIC ${DEFINES})
    endif()

    target_compile_definitions(
        ${MODULE}
        PRIVATE
            "RAINDROP_CURRENT_MODULE_NAME=\"${MODULE}\""
            "RAINDROP_CURRENT_MODULE_VERSION=1"
    )
endfunction()



# =============================
#  copy_module_clean(TARGET MODULE DESTINATION)
# =============================
function(copy_module_clean TARGET MODULE DESTINATION)
    get_target_property(OUTPUT_DIR ${MODULE} RAINDROP_OUTPUT_DIR)
    get_target_property(BINARY_DIR ${MODULE} RAINDROP_BINARY_DIR)
    get_target_property(SOURCE_DIR ${MODULE} RAINDROP_SOURCE_DIR)

    if (NOT OUTPUT_DIR)
        message(FATAL_ERROR "The module \"${MODULE}\" doesn't have a valid output directory")
    endif()

    get_filename_component(DIR_NAME ${SOURCE_DIR} NAME)

    set(MODULE_DST "${DESTINATION}/${DIR_NAME}")

    set(STAMP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${MODULE}.stamp")

    add_custom_command(
        OUTPUT ${MODULE_DST}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_DIR} ${MODULE_DST}
        DEPENDS ${MODULE}
    )

    add_custom_target(copy_${MODULE}_${TARGET}
        DEPENDS ${MODULE_DST}
        COMMENT "Cleaning and copying module \"${MODULE}\" to \"${TARGET}\" module folder"
    )

    add_dependencies(${TARGET} copy_${MODULE}_${TARGET})

endfunction()


# =============================
#  raindrop_link_modules()
# ============================
function(raindrop_link_modules)
    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)

    if (NOT MODULES)
        return()
    endif()


    foreach(MODULE IN LISTS MODULES)
        get_target_property(INTERFACE ${MODULE} RAINDROP_MODULE_INTERFACE)
        get_target_property(HARD_DEPS ${MODULE} RAINDROP_HARD_DEPS)
        get_target_property(SOFT_DEPS ${MODULE} RAINDROP_SOFT_DEPS)

        message("module ${MODULE}")
        message("hard deps : ${HARD_DEPS}")
        message("soft deps : ${SOFT_DEPS}")

        if (HARD_DEPS)
            foreach(DEP IN LISTS HARD_DEPS)
                get_target_property(DEP_INTERFACE ${DEP} RAINDROP_MODULE_INTERFACE)
                get_target_property(DEP_VERSION ${DEP} VERSION)

                if (DEP_INTERFACE)
                    
                    target_link_libraries(${INTERFACE} INTERFACE ${DEP_INTERFACE})    
                    target_compile_definitions(${MODULE} PRIVATE
                        "RAINDROP_MODULE_${DEP}_AVAILABLE=1"
                        "RAINDROP_MODULE_${DEP}_VERSION=\"${DEP_VERSION}\""
                    )
                else()  
                    message(FATAL_ERROR "[Raindrop] Module ${MODULE} is missing hard dependency ${DEP}")
                endif()
            endforeach()
        endif()

        if (SOFT_DEPS)
            foreach(DEP IN LISTS SOFT_DEPS)
                get_target_property(DEP_INTERFACE ${DEP} RAINDROP_MODULE_INTERFACE)
                get_target_property(DEP_VERSION ${DEP} VERSION)

                if (DEP_INTERFACE)
                    
                    target_link_libraries(${INTERFACE} INTERFACE ${DEP_INTERFACE})    
                    target_compile_definitions(${MODULE} PRIVATE
                        "RAINDROP_MODULE_${DEP}_AVAILABLE=1"
                        "RAINDROP_MODULE_${DEP}_VERSION=\"${DEP_VERSION}\""
                    )
                else()  
                    message(WARNING "[Raindrop] Module ${MODULE} is missing hard dependency ${DEP}")
                    target_compile_definitions(${MODULE} PRIVATE
                        "RAINDROP_MODULE_${DEP}_AVAILABLE=0"
                    )
                endif()
            endforeach()
        endif()
    endforeach()
endfunction()

function(raindrop_generate_modules_manifests)
    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)

    if (NOT MODULES)
        return()
    endif()

    foreach(MODULE IN LISTS MODULES)
        create_module_manifest(${MODULE})
    endforeach()

endfunction()