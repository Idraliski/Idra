include_directories(include)

add_library(Idra SHARED "src/Application.cpp")

# Set the output directories for the library
set_target_properties(Idra PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Export include directories
#target_include_directories(Idra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Idra/include)