cmake_minimum_required(VERSION 3.10)
INCLUDE(FindPkgConfig)

# set the project name
project(feditor)

set(CMAKE_HOST_SYSTEM_PROCESSOR arm64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB EDITOR_FILES_INCLUDE "editor/*.hpp")
file(GLOB EDITOR_FILES_SRC "editor/*.cpp")
set(EDITOR_FILES ${EDITOR_FILES_INCLUDE} ${EDITOR_FILES_SRC})

set(SOURCES main.cpp ${EDITOR_FILES})

include_directories(feditor ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(feditor ${SOURCES})

target_link_libraries(feditor ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
