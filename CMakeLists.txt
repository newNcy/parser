
project(n)


include_directories(src src/util)

add_definitions(-g -std=c99)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

#target
add_executable(main 
	src/lex.c
    src/first_set.c
    src/expression.c
	src/statement.c
    src/declaration.c
    src/parser.c
	src/main.c
	)


