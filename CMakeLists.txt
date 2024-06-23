cmake_minimum_required(VERSION 3.28.1)
project(isa_fip C)

set(CMAKE_C_STANDARD 23)

add_executable(isa_fip main.c
        /src/axvector.c
        /src/axchunk.c
        /src/axhashmap.c
        assembler.c
        preprocessor.c
        common.c
        lexer.c)

add_compile_definitions()

target_compile_options(isa_fip PRIVATE -Wall -Og -g)

target_link_libraries(isa_fip PRIVATE xxhash pcre)
