cmake_minimum_required(VERSION 3.16) # FetchContent is available in 3.11+

if(APPLE)
    set(MACOSX TRUE)
    set(CMAKE_HOST_SYSTEM_PROCESSOR arm64)
endif()

# By default clang for MacOS. GCC for Linux. It can be unify in future
if(UNIX)
    if(MACOSX)
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        set(CMAKE_C_COMPILER "/usr/bin/clang")
        message("Running on MacOS systems.") 
    else()
        set(CMAKE_CXX_COMPILER "/usr/bin/g++")
        set(CMAKE_C_COMPILER "/usr/bin/gcc")
        message("Running on Linux systems.") 
    endif()
    add_compile_definitions(UNIX)
endif()

project(feditor)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

if(UNIX)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB GUI_FILES "source/gui/*.cpp")
file(GLOB CORE_FILES "source/core/*.cpp")
file(GLOB INPUT_FILES "source/input/*.cpp")
set(SOURCE_FILES ${GUI_FILES} ${CORE_FILES} ${INPUT_FILES})
set(SOURCES "source/main.cpp" ${SOURCE_FILES})

include_directories(feditor ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "source/core")
target_include_directories(${PROJECT_NAME} PRIVATE "source/gui")
target_include_directories(${PROJECT_NAME} PRIVATE "source/input")
target_include_directories(${PROJECT_NAME} PRIVATE "source/common")
target_include_directories(feditor PRIVATE "source/thirdparty/nlohmann")

target_link_libraries(feditor ${SDL2_LIBRARIES})
target_link_libraries(feditor ${SDL2_TTF_LIBRARIES})
