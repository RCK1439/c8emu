#ifndef RAM_H
#define RAM_H

#include <stdint.h>

typedef enum memory_status_s {
    MEM_OK,
    MEM_FILE_ERR,
    MEM_ALLOC_ERR
} memory_status_t;

memory_status_t ram_init(const char *rom_file);

void ram_write(uint16_t addr, uint8_t val);
uint8_t ram_read(uint16_t addr);

#endif /* RAM_H */

