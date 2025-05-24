cmake_minimum_required(VERSION 3.27)

project(Idra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_TARGET_NAME ${PROJECT_NAME})
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_subdirectory(Idra)
add_subdirectory(Sandbox)

set_target_properties(${CMAKE_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# use the static runtime library for MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# use the dynamic runtime library for MSVC
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")

install(TARGETS ${CMAKE_TARGET_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	CONFIGURATIONS Release
)