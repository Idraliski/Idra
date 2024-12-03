include_directories(include)

# Define the editor executable
add_executable(Editor 
    "src/Editor.cpp" 
)

# Set the output directory for the executable
set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link the editor with the game engine
target_link_libraries(Editor PRIVATE Idra)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Idra> $<TARGET_FILE_DIR:Editor>
)

# Add any additional dependencies or compile definitions here