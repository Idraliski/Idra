# Define the editor executable
add_executable(Sandbox 
    "src/Sandbox.cpp" 
    "src/SandboxLayer.cpp"
    "src/Sandbox3DLayer.cpp"
)

# Set the output directory for the executable
set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Sandbox"
)

# Link the editor with the game engine
target_link_libraries(Sandbox PRIVATE Idra)

# set include directories
target_include_directories(Sandbox PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Idra
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define if in debug build
target_compile_definitions(Sandbox PRIVATE 
    $<$<CONFIG:Debug>:IDRA_DEBUG_MODE>
    GLM_ENABLE_EXPERIMENTAL
)

# Copy the Idra library to the Editor's output directory
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Idra> $<TARGET_FILE_DIR:Sandbox>
)

# put assets in the same directory as the executable
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Assets"
    "$<TARGET_FILE_DIR:Sandbox>/Assets"
)

# Add any additional dependencies or compile definitions here