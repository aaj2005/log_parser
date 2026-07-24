# interface = header only sources
add_library(structs_lib INTERFACE)

target_include_directories(structs_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})