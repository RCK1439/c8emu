#ifndef EMU_H
#define EMU_H

typedef enum emu_status_s {
    EMU_OK,
    EMU_USAGE_ERR,
    EMU_RAM_ERR,
} emu_status_t;

emu_status_t emu_run(int argc, char **argv);

#endif /* EMU_H */