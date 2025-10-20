cmake_minimum_required(VERSION 3.15)  # SFML требует минимум 3.15, не 4.0
project(GameOfLife)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Настройка SFML через FetchContent
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW TRUE
)

# Включить необходимые модули
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sfml)

add_executable(GameOfLife
        src/main.cpp
)

target_link_libraries(GameOfLife
        sfml-graphics
        sfml-window
        sfml-system
)

# Для покрытия кода
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(GameOfLife PRIVATE --coverage)
    target_link_options(GameOfLife PRIVATE --coverage)
endif()

if (MINGW)
    target_link_options(GameOfLife PRIVATE "-static" "-static-libgcc" "-static-libstdc++")
endif()
