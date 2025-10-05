cmake_minimum_required(VERSION 4.0)
project(clox C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(clox src/main.c
        src/common.h
        src/chunk.h
        src/chunk.c
        src/memory.c
        src/memory.h
        src/debug.c
        src/debug.h
        src/value.c
        src/value.h
        src/vm/vm.c
        src/vm/vm.h
        src/vm/compiler.c
        src/vm/compiler.h
        src/vm/scanner.c
        src/vm/scanner.h)
