cmake_minimum_required(VERSION 3.20)
project(shiny_finder)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(shiny_finder main.cpp
        prng.h
        stats.h)

if (OpenMP_CXX_FOUND)
    target_link_libraries(shiny_finder PUBLIC OpenMP::OpenMP_CXX)
endif ()

target_compile_options(shiny_finder PRIVATE -O3 -mavx2)