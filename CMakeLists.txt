cmake_minimum_required(VERSION 3.13)
project(ohmcha)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O3 -Wall)

file(GLOB SOURCE_FILES src/main.cpp src/model/*.cpp src/program/*.cpp)

add_executable(ohmcha ${SOURCE_FILES})

include(CMakeSpecific.txt)
