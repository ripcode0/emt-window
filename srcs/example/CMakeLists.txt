
add_executable(example example.cpp)
target_link_libraries(example PRIVATE emt::window)
target_compile_definitions(example PRIVATE NOMINMAX)