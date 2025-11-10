cmake_minimum_required(VERSION 3.15)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
        GIT_SHALLOW TRUE
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_library(GameOfLifeLib STATIC
        src/Grid.cpp
        src/Simulation.cpp
)

target_include_directories(GameOfLifeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(GameOfLifeLib PUBLIC raylib)

add_executable(GameOfLife
        src/main.cpp
)

target_link_libraries(GameOfLife PRIVATE raylib GameOfLifeLib)

# Для покрытия кода
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(GameOfLife PRIVATE --coverage)
    target_link_options(GameOfLife PRIVATE --coverage)
    target_compile_options(GameOfLifeLib PRIVATE --coverage)
    target_link_options(GameOfLifeLib PRIVATE --coverage)
endif()

if(MINGW)
    target_link_options(GameOfLife PRIVATE "-static" "-static-libgcc" "-static-libstdc++")
    target_link_options(GameOfLifeLib PRIVATE "-static" "-static-libgcc" "-static-libstdc++")
endif()

if(APPLE)
    target_link_libraries(GameOfLife PRIVATE "-framework IOKit")
    target_link_libraries(GameOfLife PRIVATE "-framework Cocoa")
    target_link_libraries(GameOfLife PRIVATE "-framework OpenGL")
endif()

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest-1.17.0/CMakeLists.txt")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest-1.17.0 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)
    else()
        message(FATAL_ERROR "googletest not found in ${CMAKE_CURRENT_SOURCE_DIR}/googletest-1.17.0. Please place googletest sources there or set BUILD_TESTS=OFF")
    endif()

    enable_testing()

    add_executable(unit_tests
        tests/test_simulation.cpp
    )
    target_link_libraries(unit_tests PRIVATE gtest_main GameOfLifeLib)
    add_test(NAME unit_tests COMMAND unit_tests)
endif()
