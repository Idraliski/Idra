cmake_minimum_required(VERSION 3.27)

project(stb)

add_library(stb STATIC)
target_include_directories(stb PUBLIC include)
target_sources(stb PRIVATE src/stb_image.cpp)
