
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
#   __check_module_not_added()
# =============================
macro(__check_module_not_added TARGET)
    if (TARGET ${TARGET})
        get_property(STAMP TARGET ${TARGET} PROPERTY RAINDROP_MODULE_STAMP)
        
        if (DEFINED STAMP)
            message(FATAL_ERROR "[Raindrop] raindrop_add_module() already called")
        endif()
    endif()
endmacro()


# =============================
#   __check_project_not_added()
# =============================
macro(__check_project_not_added TARGET)
    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "[Raindrop] unknown target \"${TARGET}\"")
    endif()

    get_property(STAMP TARGET ${TARGET} PROPERTY RAINDROP_PROJECT_STAMP)

    if (DEFINED STAMP)
        message(FATAL_ERROR "[Raindrop] raindrop_add_project() already called")
    endif()
endmacro()


# =============================
#   __raindrop_compare_version(A B COMPARATOR PASS)
# =============================
macro(__raindrop_compare_version A B COMPARATOR PASS)
    set(${PASS} FALSE)

    if("${COMPARATOR}" STREQUAL "==")
        if(${A} VERSION_EQUAL ${B})
            set(${PASS} TRUE)
        endif()
    elseif("${COMPARATOR}" STREQUAL "!=")
        if(NOT (${A} VERSION_EQUAL ${B}))
            set(${PASS} TRUE)
        endif()
    elseif("${COMPARATOR}" STREQUAL "<=")
        if(${A} VERSION_LESS_EQUAL ${B})
            set(${PASS} TRUE)
        endif()
    elseif("${COMPARATOR}" STREQUAL ">")
        if(${A} VERSION_GREATER ${B})
            set(${PASS} TRUE)
        endif()
    elseif("${COMPARATOR}" STREQUAL ">=")
        if(${A} VERSION_GREATER_EQUAL ${B})
            set(${PASS} TRUE)
        endif()
    elseif("${COMPARATOR}" STREQUAL "<")
        if(${A} VERSION_LESS ${B})
            set(${PASS} TRUE)
        endif()
    else()
        message(WARNING "[Raindrop] Unknown version comparator '${COMPARATOR}'")
    endif()
    
endmacro()


# =============================
#   __raindrop_strip_version_and_comparator(COMBINED VERSION COMPARATOR)
# =============================
macro(__raindrop_strip_version_and_comparator COMBINED VERSION COMPARATOR)
    string(REGEX MATCH "^[=<>!~]+" comparator_only "${COMBINED}")

    if(NOT comparator_only)
        set(comparator_only "==")  # No comparator found, by default equal
    endif()

    string(REGEX REPLACE "^[=<>!~]+" "" version_only "${COMBINED}")

    set(${VERSION} "${version_only}")
    set(${COMPARATOR} "${comparator_only}")
endmacro()




# =============================
# __raindrop_recursive_module_copy(TARGET MODULE DESTINATION RESOLVED)   
# =============================
function(__raindrop_recursive_module_copy TARGET MODULE DESTINATION RESOLVED)
    __raindrop_check_module_valid(${MODULE})

    
    if(DEFINED ${RESOLVED})
        set(LOCAL_RESOLVED "${${RESOLVED}}")
    else()
        set(LOCAL_RESOLVED "")
    endif()

    # early return if already resolved
    if (${MODULE} IN_LIST LOCAL_RESOLVED)
        return()
    endif()

    # push to resolved list
    list(APPEND LOCAL_RESOLVED ${MODULE})

    # setup copy target

    get_target_property(SOURCE_DIR ${MODULE} RAINDROP_SOURCE_DIR)
    get_target_property(OUTPUT_DIR ${MODULE} RAINDROP_OUTPUT_DIR)
    get_filename_component(DIR_NAME ${SOURCE_DIR} NAME)

    set(MODULE_DST "${DESTINATION}/${DIR_NAME}")

    add_custom_command(
        OUTPUT ${MODULE_DST}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_DIR} ${MODULE_DST}
        DEPENDS ${MODULE}
        COMMENT "Copying module '${MODULE}' to '${MODULE_DST}'"
    )

    add_custom_target(copy_${MODULE}_${TARGET}
        DEPENDS ${MODULE_DST}
        COMMENT "Copy target for module '${MODULE}'"
    )

    add_dependencies(${TARGET} copy_${MODULE}_${TARGET})

    # copy dependencies

    get_target_property(dependencies ${MODULE} RAINDROP_DEPENDENCIES)
    if (dependencies)
        foreach (dep IN LISTS dependencies)
            __raindrop_recursive_module_copy(${TARGET} ${dep} "${DESTINATION}" LOCAL_RESOLVED)
        endforeach()
    endif()

    set(${RESOLVED} "${LOCAL_RESOLVED}" PARENT_SCOPE)
endfunction()


# =============================
# raindrop_add_dependencies(TARGET
#   MyModuleDependency VERSION >= 0.2.3 AND <= 1.0.0
#   MyOtherDependency VERSION == 0.1.0      
# )
# =============================
function(raindrop_add_dependencies TARGET)

    # check if target is valid
    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "[Raindrop] Target '${TARGET}' does not exist")
    endif()

    # query arguments
    set(options)
    set(oneValueArgs)
    set(multiValueArgs "")

    cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

    set(unparsed_args ${ARG_UNPARSED_ARGUMENTS})

    set(current_dependency "")
    set(parsing_dependency OFF)
    set(expecting_operator OFF)

    set(dependencies "")
    # set()

    set(current_dependency ${arg})
    set(parsing_dependency ON)
    set(version_requirements "")

    # for each argument
    foreach (arg IN LISTS unparsed_args)
        # check if arg is a dependency name
        if(arg MATCHES "^[A-Z][A-Za-z0-9_]*$" AND NOT arg STREQUAL "AND" AND NOT arg STREQUAL "VERSION")
            
            if (current_dependency)
                set_target_properties(${TARGET} PROPERTIES
                    RAINDROP_DEPENDENCY_${current_dependency}_VERSION_REQUIREMENTS "${version_requirements}"
                )
            endif()

            set(version_requirements "")
            set(current_dependency ${arg})
            set(parsing_dependency ON)
            set(version_operator "=>")
            set(version_value "0.0.0")

            list(APPEND dependencies ${current_dependency})

        # if currently parsing a dependency
        elseif (parsing_dependency)

            #check if it is a version
            if (arg MATCHES "^[=<>!]+$" OR arg STREQUAL ">=" OR arg STREQUAL "<=" OR arg STREQUAL "==" OR arg STREQUAL "!=")
                set(version_operator ${arg})
                set(expecting_operator FALSE)

                continue()

            else()
                if (expecting_operator)
                    message(WARNING "[Raindrop] expected version operator")
                    # continue()
                endif()
            endif()

            if (arg STREQUAL "VERSION")

                # next argument should be operator
                set(expecting_operator TRUE)
                continue()

            elseif (arg STREQUAL "AND")

                set(expecting_operator TRUE)

                set(version_operator "")
                set(version_value "")
            
            elseif(version_operator AND arg MATCHES "^[0-9]+(\\.[0-9]+)*$")
                # Version value
                set(version_value ${arg})
                list(APPEND version_requirements "${version_operator}${version_value}")

            else()
                # Unexpected argument
                message(WARNING "[Raindrop] Unexpected argument in dependency specification: ${arg}")
            endif()

            
        else()
            message(WARNING "[Raindrop] Unexpected argument '${arg}'")
        endif()
    endforeach()

    if (current_dependency)
        set_target_properties(${TARGET} PROPERTIES
            RAINDROP_DEPENDENCY_${current_dependency}_VERSION_REQUIREMENTS "${version_requirements}"
        )
    endif()

    get_target_property(DEPENDENCIES ${TARGET} RAINDROP_DEPENDENCIES)

    if (NOT DEPENDENCIES)
        set(DEPENDENCIES "")
    endif()

    list(APPEND DEPENDENCIES "${dependencies}")
    set_target_properties(${TARGET} PROPERTIES RAINDROP_DEPENDENCIES "${DEPENDENCIES}")

endfunction()


# =============================
# raindrop_module_link_libraries(
#     PUBLIC
#        glm::glm
#        SDL2
#     PRIVATE
#        spdlog::spdlog
# )
# =============================

function(raindrop_module_link_libraries MODULE)
    __raindrop_check_module_valid(${MODULE})

    get_target_property(INTERFACE ${MODULE} RAINDROP_MODULE_INTERFACE)

    if (NOT INTERFACE)
        message(FATAL_ERROR "[Raindrop] The module '${MODULE}' is missing an interface library")
    endif()

    set(options)
    set(oneValueArgs)
    set(multiValueArgs PUBLIC PRIVATE)
    
    cmake_parse_arguments(
        ARG
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if (ARG_PUBLIC)
        target_link_libraries(${INTERFACE} INTERFACE "${ARG_PUBLIC}")
    endif()

    if (ARG_PRIVATE)
        target_link_libraries(${MODULE} PRIVATE "${ARG_PRIVATE}")
    endif()

endfunction()

# =============================
#  raindrop_add_module(
#     (OPTIONAL) CRITICAL
#     (REQUIRED) MODULE 
#     (REQUIRED) VERSION 0.0.1
#     (OPTIONAL) DESCRIPTION "Lorem Ipsum" 
#     (REQUIRED) SOURCE sources
#     (OPTIONAL) HEADERS headers 
# )
# =============================

function(raindrop_add_module MODULE)
    __check_module_not_added(${MODULE})

    set(options CRITICAL)
    set(oneValueArgs VERSION DESCRIPTION)
    set(multiValueArgs SOURCE HEADERS)
    
    cmake_parse_arguments(
        ARG
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    # ============ Checking arguments

    if(NOT ARG_VERSION)
        message(FATAL_ERROR "[Raindrop] argument 'VERSION' is required for raindrop_add_module()")
    endif()

    if(NOT ARG_SOURCE)
        message(FATAL_ERROR "[Raindrop] argument 'SOURCE' is required for add_raindrop_module()")
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
        RAINDROP_MODULE_CRITICAL "${ARG_CRITICAL}"
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

    set(data_folder "${CMAKE_CURRENT_SOURCE_DIR}/data/")
    set(data_output_folder "${OUTPUT_DIR}/data")

    if (EXISTS "${data_folder}")
        file(GLOB_RECURSE src_files "${data_folder}/*")

        add_custom_command(
            OUTPUT "${BINARY_DIR}/data.stamp"
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_folder} ${data_output_folder}
            COMMAND ${CMAKE_COMMAND} -E touch "${BINARY_DIR}/data.stamp"
            DEPENDS ${src_files}
            COMMENT "Copying ${data_folder} to ${data_output_folder}"
        )
    endif()

    set_property(GLOBAL PROPERTY RAINDROP_MODULE_${MODULE} ${MODULE})

    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)
    list(APPEND MODULES ${MODULE})
    set_property(GLOBAL PROPERTY RAINDROP_MODULES ${MODULES})

    set(${MODULE} ${MODULE} PARENT_SCOPE)

endfunction()


# =============================
#  __raindrop_link_modules()
# ============================
function(__raindrop_link_modules)
    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)

    if (NOT MODULES)
        return()
    endif()


    foreach(MODULE IN LISTS MODULES)
        get_target_property(INTERFACE ${MODULE} RAINDROP_MODULE_INTERFACE)
        get_target_property(DEPS ${MODULE} RAINDROP_DEPENDENCIES)

        if (DEPS)
            foreach(DEP IN LISTS DEPS)
                get_target_property(DEP_INTERFACE ${DEP} RAINDROP_MODULE_INTERFACE)
                get_target_property(DEP_VERSION ${DEP} VERSION)

                if (DEP_INTERFACE)
                    target_link_libraries(${INTERFACE} INTERFACE ${DEP_INTERFACE})
                else()  
                    message(FATAL_ERROR "[Raindrop] Module ${MODULE} is missing dependency ${DEP}")
                endif()
            endforeach()
        endif()
    endforeach()
endfunction()


# =============================
#  __raindrop_create_module_manifest(NAME)
# =============================
function(__raindrop_create_module_manifest MODULE)
    get_target_property(VERSION ${MODULE} VERSION)
    get_target_property(DESCRIPTION ${MODULE} DESCRIPTION)
    get_target_property(DEPS ${MODULE} RAINDROP_DEPENDENCIES)
    get_target_property(OUTPUT_DIR ${MODULE} RAINDROP_OUTPUT_DIR)
    get_target_property(CRITICAL ${MODULE} RAINDROP_MODULE_CRITICAL)

    # === Validation
    if (NOT VERSION)
        message(FATAL_ERROR "Module ${MODULE} is missing a version")
    endif()

    if (NOT DESCRIPTION)
        set(DESCRIPTION "")
    endif()

    if (NOT DEPS)
        set(DEPS "")
    endif()


    # === Format dependencies
    # == HARD
    set(DEPS_JSON "")

    foreach (dep IN LISTS DEPS)
        list(GET dep 0 dep_name)
        string(APPEND DEPS_JSON "\t\t\"${dep_name}\": \"=>0.0.0\",\n")
    endforeach()

    if (DEPS_JSON)
        # remove trailing comma
        string(REGEX REPLACE ",\n$" "\n" DEPS_JSON "${DEPS_JSON}") 
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
    set(MODULE_DEPENDENCIES ${DEPS_JSON})

    if (CRTICICAL)
        set(MODULE_CRITICAL true)
    else()
        set(MODULE_CRITICAL false)
    endif()


    set(MANIFEST_OUTPUT "${OUTPUT_DIR}/manifest.json")

    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/templates/moduleManifest.json.in"
        ${MANIFEST_OUTPUT}
        @ONLY
    )
endfunction()


# =============================
#  __raindrop_generate_modules_manifests()
# ============================
function(__raindrop_generate_modules_manifests)
    get_property(MODULES GLOBAL PROPERTY RAINDROP_MODULES)

    if (NOT MODULES)
        return()
    endif()

    foreach(MODULE IN LISTS MODULES)
        __raindrop_create_module_manifest(${MODULE})
    endforeach()

endfunction()


# =============================
#  raindrop_finalize_modules()
# ============================
function(raindrop_finalize_modules)
    __raindrop_generate_modules_manifests()
    __raindrop_link_modules()
endfunction()



# =============================
#  raindrop_add_project(PROJECT)
# =============================

function(raindrop_add_project TARGET)
    __check_project_not_added(${TARGET})

    message(STATUS "[Raindrop] New project '${TARGET}'")
    message(STATUS "[Raindrop] Cheking dependencies...")

    # get and check dependencies
    get_target_property(dependencies ${TARGET} RAINDROP_DEPENDENCIES)
    get_property(available_modules GLOBAL PROPERTY RAINDROP_MODULES)

    list(LENGTH dependencies dependency_count)
    if (dependency_count EQUAL 0)
        message(STATUS "[Raindrop] Not requiering any dependency")
    else()
        message(STATUS "[Raindrop] Requiering ${dependency_count} dependency(ies)")
        foreach (dependency IN LISTS dependencies)
            message("\t- ${dependency}")
        endforeach()
    endif()

    # check dependencies and their versions
    foreach(dependency IN LISTS dependencies)
        if (NOT ${dependency} IN_LIST available_modules)
            message(FATAL_ERROR "[Raindrop] Project '${TARGET}' is missing dependency ${dependency}")
        endif()

        get_target_property(dependency_version ${dependency} VERSION)
        get_target_property(version_requirements ${TARGET} RAINDROP_DEPENDENCY_${dependency}_VERSION_REQUIREMENTS)

        foreach (requirement IN LISTS version_requirements)
            __raindrop_strip_version_and_comparator(${requirement} required_version comparator)
            __raindrop_compare_version(${dependency_version} ${required_version} ${comparator} pass)

            if (NOT pass)
                message(FATAL_ERROR "[Raindrop] Project '${TARGET}' has a missmatching dependency version (required ${dependency} version ${comparator} ${required_version} and got ${dependency_version})")
            endif()

        endforeach()

        # link the dependency
        get_target_property(dependency_interface ${dependency} RAINDROP_MODULE_INTERFACE)

        target_link_libraries(${TARGET} PRIVATE ${dependency_interface})
    endforeach()

    # copy the modules to the projet's folder

    # Directory where the executable is located
    set(TARGET_FILE $<TARGET_FILE:${TARGET}>)
    get_target_property(TARGET_DIR ${TARGET} RUNTIME_OUTPUT_DIRECTORY)

    if (NOT TARGET_DIR)
        set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    set(resolved "")
    foreach (dep IN LISTS dependencies)
        __raindrop_recursive_module_copy(${TARGET} ${dep} "${TARGET_DIR}/modules/" resolved)
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
    set_property(TARGET ${TARGET} PROPERTY RAINDROP_PROJECT_STAMP TRUE)

    set_target_properties(${TARGET} PROPERTIES
        BUILD_RPATH "\$ORIGIN"
        INSTALL_RPATH "\$ORIGIN"
    )

    message(STATUS "Raindrop project ${TARGET} set up with success !")

endfunction()