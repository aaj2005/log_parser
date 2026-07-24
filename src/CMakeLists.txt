# create log_parser lib
add_library(log_parser STATIC log_parser.cpp)

# link the structs lib
target_link_libraries(log_parser PRIVATE structs_lib)


# expose the log_parser directory
target_include_directories(log_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})