cmake_minimum_required(VERSION 3.20)
project(shiny-finder)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(shiny-finder main.cpp
        prng.h
        stats.h)

if (OpenMP_CXX_FOUND)
    target_link_libraries(shiny-finder PUBLIC OpenMP::OpenMP_CXX)
endif ()

target_compile_options(shiny-finder PRIVATE -O3 -mavx2)