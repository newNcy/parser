
project(n)

cmake_minimum_required(VERSION 3.14)

include_directories(src)
add_definitions(-g)

add_executable(main 
	src/lex.c
    src/expression.c
    src/declaration.c
    src/first_set.c
    src/parser.c
	src/main.c
	)


