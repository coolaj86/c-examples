cmake_minimum_required(VERSION 2.6)

add_definitions (-Wall -Werror -ggdb3)

add_library (pseudofd STATIC pseudo-fd.c)
# add_library (pseudofd SHARED pseudo-fd.c)

add_executable(pseudofd-test pseudo-fd-test.c)
target_link_libraries(pseudofd-test pseudofd)
