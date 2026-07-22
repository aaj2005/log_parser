# set the minimum cmake version
cmake_minimum_required(VERSION 3.25)


project(log parser
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(src)


