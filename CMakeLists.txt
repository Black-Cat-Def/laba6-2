cmake_minimum_required(VERSION 3.16)
project(laba6_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba6_2 main.cpp
        container.cpp
        container.h
        gpu.cpp
        gpu.h
        sqlite3.c
        sqlite3.h
        sqlite_container.cpp
        sqlite_container.h
        gpu_strategy.h
        gpu_strategy.cpp)