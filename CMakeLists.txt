cmake_minimum_required(VERSION 3.12)
project(c8emu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(c8emu_VERSION_MAJOR 0)
set(c8emu_VERSION_MINOR 6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(src)
add_subdirectory(vendor)