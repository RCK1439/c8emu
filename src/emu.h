#ifndef EMU_H
#define EMU_H

#include <stdint.h>

typedef enum EmulatorResult
{
    EMU_OK,
    EMU_USAGE_ERR,
    EMU_RAM_ERR,
} EmulatorResult;

EmulatorResult RunEmulator(int32_t argc, char **argv);

#endif /* EMU_H */
