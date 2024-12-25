#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
#include "instructions.h"

#include <stddef.h>
#include <stdint.h>

#define DISASSEMBLE(rom_name, prog, size) Disassemble(rom_name, prog, size)
#define DEBUG_OPCODE(op) DebugOpCode(op)
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp) DrawDebugInfo(v, dt, st, idx, pc, kp);

void Disassemble(const char *romName, const uint8_t *const prog, size_t size);
void DebugOpCode(const OpCode *const op);
void DrawDebugInfo(const uint8_t *const v, uint8_t dt, uint8_t st, uint16_t idx, uint16_t pc, const uint8_t *const kp);

#else
#define DISASSEMBLE(rom_name, prog, size)
#define DEBUG_OPCODE(op)
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp)
#endif

#endif /* DEBUG_H */

