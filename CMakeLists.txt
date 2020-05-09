cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(sim)

file(GLOB_RECURSE SRC_FILES_sim src/*.cpp)
add_executable(sim ${SRC_FILES_sim})
target_compile_options(sim PRIVATE -std=c++17 -g -Wall -Werror)
target_include_directories(sim PRIVATE src)
target_link_libraries(sim sfml-system sfml-window sfml-graphics pthread)

