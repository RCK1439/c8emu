#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
#include "instructions.h"

#include <stdint.h>
#include <stddef.h>

#define DISASSEMBLE(rom_name, prog, size) disassemble(rom_name, prog, size)
#define DEBUG_OPCODE(op) debug_opcode(op)

void disassemble(const char *rom_name, uint8_t *prog, size_t size);
void debug_opcode(opcode_t *op);
#else
#define DISASSEMBLE(rom_name, prog, size)
#define DEBUG_OPCODE(op)
#endif

#endif /* DEBUG_H */