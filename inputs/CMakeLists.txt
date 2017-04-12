cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Inputs)

add_executable(inputs print_int.cpp square_int.cpp Turtle.cpp macro_city.cpp)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
