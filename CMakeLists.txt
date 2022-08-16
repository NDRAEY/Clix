cmake_minimum_required(VERSION 3.23)

project(clix)

set(CMAKE_C_FLAGS "-ggdb -Wall -O0")

set(OUTPUT clix)
set(LIBFILES src/tokenizer.c src/lexer.c src/actionlist.c
			 src/executor.c)
set(EXTERNAL thirdparty/ValeraC/valera.c)

add_executable(${OUTPUT} src/main.c ${LIBFILES} ${EXTERNAL})

