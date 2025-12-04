

include("${CMAKE_SOURCE_DIR}/cmake/RaindropModule.cmake")

file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
    
# Filter to only directories
set(dir_list "")
foreach(subdir ${subdirectories})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        list(APPEND dir_list ${subdir})
    endif()
endforeach()

# Add each subdirectory
foreach(subdir ${dir_list})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory(${subdir})
    else()
        message(STATUS "Skipping ${subdir} (no CMakeLists.txt)")
    endif()
endforeach()

function(link_modules)
    include("${CMAKE_SOURCE_DIR}/cmake/RaindropModuleHelper.cmake")
    raindrop_generate_modules_manifests()
    raindrop_link_modules()
endfunction()

link_modules()