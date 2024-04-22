#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
#include <stdint.h>
#include <stddef.h>

void disassemble(const char *rom_name, uint8_t *prog, size_t size);
#endif

#endif /* DEBUG_H */