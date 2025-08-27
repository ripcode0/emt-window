cmake_minimum_required(VERSION 3.22)

project(emt-window LANGUAGES C CXX
        VERSION 1.0.0
        DESCRIPTION "window api wrapper"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(srcs/emt/window)

add_subdirectory(srcs/example)

# add_executable(game ref/cmd_prog.cpp)