cmake_minimum_required(VERSION 3.11)

project(clix VERSION 0.0.8)

set(CMAKE_C_FLAGS "-ggdb -Wall -O3") # Will be removed after version 1.0

set(OUTPUT clix)
set(LIBFILES src/tokenizer.c src/lexer.c src/actionlist.c
			 src/executor.c src/error.c src/ffi.c)
set(EXTERNAL thirdparty/ValeraC/valera.c)

add_executable(${OUTPUT} src/main.c ${LIBFILES} ${EXTERNAL})

