# set the minimum cmake version
cmake_minimum_required(VERSION 3.25)
project(logparser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # force the use of a specific c++ version
set(CMAKE_CXX_EXTENSIONS OFF) # if ON, it will use -std=gnu++ which we don't want

if (CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
	add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -O3)
endif()

add_subdirectory(src)
add_subdirectory(lib)


add_executable(log_analyser main.cpp)

target_link_libraries(log_analyser PRIVATE log_parser structs_lib)
