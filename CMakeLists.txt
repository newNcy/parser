
project(n)

cmake_minimum_required(VERSION 3.14)

include_directories(src)
add_definitions(-g)

add_executable(main 
	src/main.c
	src/lex.c
	)


