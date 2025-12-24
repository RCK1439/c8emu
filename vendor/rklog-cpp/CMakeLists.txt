cmake_minimum_required(VERSION 3.12)
project(rklog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(rklog_VERSION_MAJOR 1)
set(rklog_VERSION_MINOR 0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(rklog_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LogImpl.cpp
)
set(rklog_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Config/Color.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Config/Config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Config/Level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Config/Style.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Core/Platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Core/Time.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Logger/BasicLogger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rklog/Logger/Logger.hpp

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
