cmake_minimum_required(VERSION 3.22)
project(RISC_V_Simulator C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(code
        main.cpp
        predictor.h)
