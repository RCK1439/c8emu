#pragma once

#include "core/platform.hpp"

// --- utility macros ---------------------------------------------------------

#define STR(x)       STRINGIFY(x)
#define STRINGIFY(x) #x

// --- version ----------------------------------------------------------------

#define C8_VERSION_MAJOR 0
#define C8_VERSION_MINOR 6

#define C8_VERSION_STRING "v" STR(C8_VERSION_MAJOR) "." STR(C8_VERSION_MINOR)

// --- window details ---------------------------------------------------------

#define C8_PROG_NAME "c8emu"

#if defined(C8_DEBUG)
#define C8_WINDOW_TITLE C8_PROG_NAME "-" C8_VERSION_STRING "-debug"
#else
#define C8_WINDOW_TITLE C8_PROG_NAME "-" C8_VERSION_STRING
#endif
template<typename T>
constexpr T C8_WINDOW_WIDTH = 1024;
template<typename T>
constexpr T C8_WINDOW_HEIGHT = 512;

