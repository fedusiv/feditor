cmake_minimum_required(VERSION 3.10)

# set the project name
project(feditor)

set(CMAKE_HOST_SYSTEM_PROCESSOR arm64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("editor")

add_executable(feditor main.cpp)
target_link_libraries(feditor editor)
target_link_libraries(feditor ${SDL2_LIBRARIES})
