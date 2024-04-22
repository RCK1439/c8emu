#ifndef RAM_H
#define RAM_H

#include <stdint.h>

typedef enum ram_status_s {
    RAM_OK,
    RAM_FILE_ERR,
    RAM_ALLOC_ERR
} ram_status_t;

ram_status_t ram_init(const char *rom_file);

void ram_write(uint16_t addr, uint8_t val);
uint16_t ram_read(uint16_t addr);

#endif /* RAM_H */