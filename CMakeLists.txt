cmake_minimum_required(VERSION 3.29)
project(dir_in_c C)

set(CMAKE_C_STANDARD 99)

add_executable(dir_in_c
        main.c
        c_unit.c
        dir_in_c.c
)