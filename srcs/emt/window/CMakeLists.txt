file(GLOB_RECURSE srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(emt-window STATIC ${srcs})
add_library(emt::window ALIAS emt-window)

get_filename_component(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

target_include_directories(emt-window PUBLIC "${include_dir}")

target_link_libraries(emt-window PRIVATE gdiplus d2d1 dwrite)