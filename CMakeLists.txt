cmake_minimum_required(VERSION 3.16)
project(RISC_V_Simulator)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(1-seq)

add_executable(code
        1-seq/main.cpp)
