#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef enum Chip8ExitCode
{
    C8_OK,
    C8_USAGE_ERR,
    C8_RAM_ERR,
    C8_PANIC,
} Chip8ExitCode;

Chip8ExitCode InvokeChip8(int32_t argc, char **argv);

#endif /* CHIP8_H */
