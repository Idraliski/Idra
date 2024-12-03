include_directories(include)

# Define the editor executable
add_executable(Sandbox 
    "src/Editor.cpp" 
    "src/Entrypoint.cpp"
)

# Set the output directory for the executable
set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link the editor with the game engine
target_link_libraries(Sandbox PRIVATE Idra)

# Ensure the game engine headers are available
target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Idra/include)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Idra> $<TARGET_FILE_DIR:Sandbox>
)

# Add any additional dependencies or compile definitions here