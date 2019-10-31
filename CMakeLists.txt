
project(n)


include_directories(src)

add_definitions(-g -std=c99)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

#target
add_executable(main 
	src/queue.c
    src/expression.c
    src/declaration.c
    src/first_set.c
    src/parser.c
	src/main.c
	src/lex.c
	)


