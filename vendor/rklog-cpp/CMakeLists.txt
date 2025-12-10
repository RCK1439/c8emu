cmake_minimum_required(VERSION 3.12)
project(rklog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(rklog_VERSION_MAJOR 0)
set(rklog_VERSION_MINOR 1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(rklog_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rklog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
)
set(rklog_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/style.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/core/time.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/rklog.hpp
)

add_library(rklog STATIC ${rklog_HEADERS} ${rklog_SOURCES})

target_compile_definitions(rklog PRIVATE NDEBUG)
if(MSVC)
    target_compile_options(rklog PRIVATE /WX /W4)
else()
    target_compile_options(rklog PRIVATE -Wall -Werror -Wextra -Wpedantic)
endif()

target_include_directories(rklog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set_target_properties(rklog PROPERTIES
    OUTPUT_NAME "rklog"
    VERSION ${rklog_VERSION_MAJOR}.${rklog_VERSION_MINOR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
