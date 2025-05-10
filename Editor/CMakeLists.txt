# Define the editor executable
add_executable(Editor 
    "src/Editor.cpp" 
    "src/ExampleLayer.cpp"
)

# Set the output directory for the executable
set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link the editor with the game engine
target_link_libraries(Editor PRIVATE Idra)

# set include directories
target_include_directories(Editor PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Idra
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define if in debug build
target_compile_definitions(Editor PRIVATE 
    $<$<CONFIG:Debug>:IDRA_DEBUG_MODE>
    GLM_ENABLE_EXPERIMENTAL
)

# Copy the Idra library to the Editor's output directory
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Idra> $<TARGET_FILE_DIR:Editor>
)

# put assets in the same directory as the executable
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Assets"
    "$<TARGET_FILE_DIR:Editor>/Assets"
)

# Add any additional dependencies or compile definitions here