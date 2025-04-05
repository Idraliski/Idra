add_subdirectory(vendor)

add_library(Idra SHARED 
    "src/Core/Application.cpp"
    "src/Core/Log.cpp"
    "src/IdraPCH.cpp" 
    "src/Platform/Windows/WindowsWindow.cpp"
    "src/Core/Layer.cpp" 
    "src/Core/LayerStack.cpp"
    "src/ImGui/ImGuiLayer.cpp"
    "src/Platform/OpenGL/ImGuiOpenGLRenderer.cpp"
    "src/Platform/Windows/WindowsInput.cpp"
)

# Set the output directories for the library
set_target_properties(Idra PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# set macro for exporting symbols for the engine
target_compile_definitions(Idra PRIVATE 
    IDRA_CORE_EXPORT 
    GLFW_INCLUDE_NONE
    IDRA_DEBUG
)

# link the vendor libraries
target_link_libraries(Idra PUBLIC spdlog glad glfw imgui)

# Precompile headers
target_precompile_headers(Idra PRIVATE "include/IdraPCH.h")

# Export include directories
target_include_directories(Idra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)