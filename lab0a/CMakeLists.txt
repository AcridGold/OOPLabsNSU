cmake_minimum_required(VERSION 4.0)
project(lab0)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab0 main.cpp
        module1.h
        module1.cpp
        module2.h
        module2.cpp)
