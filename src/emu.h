#ifndef EMU_H
#define EMU_H

#include <stdint.h>

typedef enum emulator_status_s {
    EMU_OK,
    EMU_USAGE_ERR,
    EMU_RAM_ERR,
} emulator_status_t;

emulator_status_t emu_run(int32_t argc, char **argv);

#endif /* EMU_H */
