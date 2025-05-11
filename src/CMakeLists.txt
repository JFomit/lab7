cmake_minimum_required(VERSION 3.20)

add_executable(runner main.cpp)

target_compile_options(runner PRIVATE -fsanitize=address)
target_link_options(runner PRIVATE -fsanitize=address)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
target_compile_definitions(runner PRIVATE DEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)