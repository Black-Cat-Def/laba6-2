cmake_minimum_required(VERSION 4.2)
project(laba5_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(laba5_2 main.cpp
        gpu.cpp
        gpu.h
        container.h
        container.cpp
        sqlite3.c
        sqlite3.h
        sqlite_container.h
        sqlite_container.cpp)
