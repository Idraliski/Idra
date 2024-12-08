include_directories(
    include
)
add_subdirectory(vendor)

add_library(Idra SHARED 
    "src/Core/Application.cpp"
    "src/Core/Log.cpp"
)

# Set the output directories for the library
set_target_properties(Idra PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# set macro for exporting symbols for the engine
target_compile_definitions(Idra PRIVATE IDRA_CORE_EXPORT)

target_link_libraries(Idra PUBLIC spdlog)

# Export include directories
target_include_directories(Idra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})