cmake_minimum_required(VERSION 3.10)

find_package(SDL2 REQUIRED)
include_directories(editor ${SDL2_INCLUDE_DIRS})
add_library(editor SHARED window.cpp)
target_link_libraries(editor ${SDL2_LIBRARIES})