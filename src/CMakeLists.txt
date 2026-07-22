# create an executable called log_parser
add_executable(log_parser)

# the executable depends on the log_parser.cpp source file
target_sources(log_parser PRIVATE log_parser.cpp)

target_compile_features(log_parser PUBLIC cxx_std_23)
