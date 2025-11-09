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

add_executable(GameOfLife
        src/main.cpp
        src/Grid.cpp
        src/Simulation.cpp
)

target_link_libraries(GameOfLife PRIVATE raylib)

# Для покрытия кода
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(GameOfLife PRIVATE --coverage)
    target_link_options(GameOfLife PRIVATE --coverage)
endif()

if(MINGW)
    target_link_options(GameOfLife PRIVATE "-static" "-static-libgcc" "-static-libstdc++")
endif()

if(APPLE)
    target_link_libraries(GameOfLife PRIVATE "-framework IOKit")
    target_link_libraries(GameOfLife PRIVATE "-framework Cocoa")
    target_link_libraries(GameOfLife PRIVATE "-framework OpenGL")
endif()
