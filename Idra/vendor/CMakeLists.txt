add_subdirectory(spdlog)
add_subdirectory(glfw-3.4)
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(glm)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(assimp)