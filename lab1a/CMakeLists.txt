cmake_minimum_required(VERSION 3.14)
project(lab1a)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение coverage флагов
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_executable(lab1a
        BitArray.cpp
        BitArray.h
        main.cpp
)

add_subdirectory(googletest-1.17.0)

add_executable(test_lab1a
        test_main.cpp
        BitArray.cpp
        BitArray.h
)

target_link_libraries(test_lab1a PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(test_lab1a)