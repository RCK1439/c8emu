cmake_minimum_required(VERSION 3.12)
project(c8emu LANGUAGES C)

# Build mode configuration
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)

# Platform detection
if(WIN32)
    set(PLATFORM "WINDOWS")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM "LINUX")
elseif(APPLE)
    set(PLATFORM "MACOS")
    message(FATAL_ERROR "MacOS not yet supported")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Compiler configuration
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Common compiler flags
add_compile_options(
    -Wall
    -Werror
    -Wpedantic
    -Wextra
)

# Configuration-specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    add_compile_options(-O3 -march=native)
    if(PLATFORM STREQUAL "WINDOWS")
        add_compile_definitions(CRT_SECURE_NO_WARNINGS)
        add_compile_options(-mwindows)
    endif()
else()
    add_compile_options(-O0 -ggdb)
endif()

# Source files
file(GLOB_RECURSE SRC_FILES 
    src/*.c 
    src/client/*.c 
    src/core/*.c 
    src/emulator/*.c 
    src/renderer/*.c
)

# Output directories
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})

# Third-party includes
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(RAYLIB_INCL ${VENDOR_DIR}/raylib/include)
set(RKLOG_INCL ${VENDOR_DIR}/rklog)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${RAYLIB_INCL}
    ${RKLOG_INCL}
)

# Platform-specific linking
if(PLATFORM STREQUAL "WINDOWS")
    set(RAYLIB_LINK ${VENDOR_DIR}/raylib/lib/windows)
    set(LIBS raylib winmm user32 shell32 gdi32)
elseif(PLATFORM STREQUAL "LINUX")
    set(RAYLIB_LINK ${VENDOR_DIR}/raylib/lib/linux)
    set(LIBS raylib m)
endif()

link_directories(${RAYLIB_LINK})

# Executable target
add_executable(c8emu ${SRC_FILES})

if(PLATFORM STREQUAL "WINDOWS")
    set_target_properties(c8emu PROPERTIES SUFFIX ".exe")
endif()

target_link_libraries(c8emu ${LIBS})