
project(n)
cmake_minimum_required(VERSION 2.8)

include_directories(src src/util)

add_definitions(-g -std=c99)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_library(util STATIC 
	src/util/map.c
	src/util/vector.c
	)
#target
add_executable(main 
	src/lex.c
    src/first_set.c
    src/declaration.c
	src/expression.c
    src/parser.c
	src/env.c
	src/main.c
	)

target_link_libraries(main util)


