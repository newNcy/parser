
project(n)

cmake_minimum_required(VERSION 3.14)

include_directories(src)

add_definitions(-g)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

#target
add_executable(main 
	src/lex.c
    src/expression.c
    src/declaration.c
    src/first_set.c
    src/parser.c
	src/main.c
	)


