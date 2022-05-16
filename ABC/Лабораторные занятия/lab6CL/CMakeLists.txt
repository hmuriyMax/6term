cmake_minimum_required(VERSION 3.21)
project(lab6CL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(lab6CL main.cpp)
