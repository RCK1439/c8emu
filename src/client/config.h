#ifndef CONFIG_H
#define CONFIG_H

#include "core/platform.h"

#define STR(x)       STRINGIFY(x)
#define STRINGIFY(x) #x

#define C8_VERSION_MAJOR 1
#define C8_VERSION_MINOR 5

#define C8_VERSION_STRING "v"STR(C8_VERSION_MAJOR)"."STR(C8_VERSION_MINOR)

#define C8_PROG_NAME "c8emu"

#if defined(C8_DEBUG)
#   define C8_WINDOW_TITLE C8_PROG_NAME" - "C8_VERSION_STRING"-debug"
#else
#   define C8_WINDOW_TITLE C8_PROG_NAME" - "C8_VERSION_STRING
#endif
#define C8_WINDOW_WIDTH 1024
#define C8_WINDOW_HEIGHT 512

#define C8_TARGET_FPS 60

#endif /* CONFIG_H */
