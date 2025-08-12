cmake_minimum_required(VERSION 3.22)

project(emt-window LANGUAGES C CXX
        VERSION 1.0.0
        DESCRIPTION "window api wrapper"
)

add_subdirectory(srcs/emt/window)

add_subdirectory(srcs/example)