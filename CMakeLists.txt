cmake_minimum_required(VERSION 3.26)
project(Homework5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Homework5
        BaseFilters-1.c
        BmpProcessor.h
        PixelProcessor.h
        BmpProcessor.c)
