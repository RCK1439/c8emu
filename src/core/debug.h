#ifndef DEBUG_H
#define DEBUG_H

#include "platform.h"

#if defined(C8_DEBUG)
#include "types.h"

#include "emulator/instructions.h"

#define DISASSEMBLE(rom_name, prog, size)       c8Disassemble(rom_name, prog, size)
#define DEBUG_OPCODE(op)                        c8DebugOpCode(op)
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp) c8DrawDebugInfo(v, dt, st, idx, pc, kp);

void c8Disassemble(const char *romName, const u8 *prog, size_t size);
void c8DebugOpCode(const Chip8OpCode *op);
void c8DrawDebugInfo(const u8 *v, u8 dt, u8 st, u16 idx, u16 pc, const u8 *kp);

#else
#define DISASSEMBLE(rom_name, prog, size)       (void)0
#define DEBUG_OPCODE(op)                        (void)0
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp) (void)0
#endif

#endif /* DEBUG_H */

