cmake_minimum_required(VERSION 3.12)
project(c8emu)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(c8emu_VERSION_MAJOR 1)
set(c8emu_VERSION_MINOR 6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(src)
add_subdirectory(vendor)