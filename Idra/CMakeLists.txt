add_subdirectory(vendor)

add_library(Idra STATIC 
    "src/IdraPCH.cpp" 
    "src/Core/Application.cpp"
    "src/Core/Layer.cpp" 
    "src/Core/LayerStack.cpp"
    "src/Core/Log.cpp"
    "src/ImGui/ImGuiLayer.cpp"
    "src/ImGui/ImGuiBuild.cpp"
    "src/Platform/OpenGL/OpenGLContext.cpp"
    "src/Platform/Windows/WindowsInput.cpp"
    "src/Platform/Windows/WindowsWindow.cpp"
    "src/Renderer/Shader.cpp"
    "src/Renderer/Buffer.cpp" 
    "src/Renderer/Renderer.cpp"
    "src/Platform/OpenGL/OpenGLBuffer.cpp" 
    "src/Renderer/VertexArray.cpp" 
    "src/Platform/OpenGL/OpenGLVertexArray.cpp")

# Set the output directories for the library
set_target_properties(Idra PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# set macro for exporting symbols for the engine
target_compile_definitions(Idra PRIVATE 
    GLFW_INCLUDE_NONE
    $<$<CONFIG:DEBUG>:IDRA_DEBUG_MODE>
)

# link the vendor libraries
target_link_libraries(Idra PUBLIC spdlog glad glfw imgui glm)

# Precompile headers
target_precompile_headers(Idra PRIVATE "include/IdraPCH.h")

# Export include directories
target_include_directories(Idra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)