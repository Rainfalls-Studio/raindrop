set(PCH_TARGET "${CMAKE_CURRENT_BINARY_DIR}/precompiled_common.pch")
add_library(pch_target INTERFACE)
target_precompile_headers(pch_target INTERFACE ${PCH_PATH})

# === ENGINE ===

file(GLOB_RECURSE SOURCES *.cpp)
add_library(RaindropEngine STATIC ${SOURCES})

target_link_libraries(RaindropEngine PRIVATE ${Vulkan_LIBRARIES} SDL2::SDL2-static assimp::assimp)
target_link_libraries(RaindropEngine PUBLIC easyloggingpp tinyxml2 imgui pch_target)

set_target_properties(RaindropEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)