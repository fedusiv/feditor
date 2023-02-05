cmake_minimum_required(VERSION 3.20)
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

# This mess with adding src files need to be fixed
file(GLOB CORE_FILES_INCLUDE "source/core/*.hpp")
file(GLOB CORE_FILES_SRC "source/core/*.cpp")
set(CORE_FILES ${CORE_FILES_INCLUDE} ${CORE_FILES_SRC})

file(GLOB GUI_FILES_INCLUDE "source/gui/*.hpp")
file(GLOB GUI_FILES_SRC "source/gui/*.cpp")
set(GUI_FILES ${GUI_FILES_INCLUDE} ${GUI_FILES_SRC})

file(GLOB INPUT_FILES_INCLUDE "source/input/*.hpp")
file(GLOB INPUT_FILES_SRC "source/input/*.cpp")
set(INPUT_FILES ${INPUT_FILES_INCLUDE} ${INPUT_FILES_SRC})

file(GLOB COMMON_FILES_INCLUDE "source/common/*.hpp")
file(GLOB COMMON_FILES_SRC "source/common/*.cpp")
set(COMMON_FILES ${COMMON_FILES_INCLUDE} ${COMMON_FILES_SRC})

set(SOURCE_FILES ${COMMON_FILES} ${GUI_FILES} ${INPUT_FILES} ${CORE_FILES})

set(SOURCES "source/main.cpp" ${SOURCE_FILES})

include_directories(feditor ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(feditor ${SOURCES})
target_link_libraries(feditor ${SDL2_LIBRARIES})
target_link_libraries(feditor ${SDL2_TTF_LIBRARIES})
