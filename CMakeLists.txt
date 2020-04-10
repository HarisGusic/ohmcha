cmake_minimum_required(VERSION 3.13)
project(ohmcha)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp src/*/*.cpp)

add_executable(ohmcha ${SOURCE_FILES})