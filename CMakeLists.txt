cmake_minimum_required(VERSION 3.20)
project(OpenMP)

set(CMAKE_CXX_STANDARD 14)

add_executable(OpenMP main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")