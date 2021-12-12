cmake_minimum_required(VERSION 3.16)

project(ads-lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(
	-Wall
	-Wextra
	-Wpedantic
)

add_subdirectory(src)

add_subdirectory(test)
