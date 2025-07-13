#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

// --- general ----------------------------------------------------------------

#define C8_NUM_KEYS (16)

// --- cpu --------------------------------------------------------------------

#define C8_NUM_REGISTERS (16)

#define C8_SCREEN_BUFFER_WIDTH (64)
#define C8_SCREEN_BUFFER_HEIGHT (32)

// --- memory -----------------------------------------------------------------

#define C8_FONTSET_SIZE (80)
#define C8_MEMORY_SIZE (4 * 1024)
#define C8_MAX_ROM_SIZE (C8_MEMORY_SIZE - 512)

#define C8_ADDR_ROM    (0x0200) // Starting address for ROM data
#define C8_ADDR_SCREEN (0x0F00) // Starting address for Screen
#define C8_ADDR_STACK  (0x0FA0) // Starting address for call stack
#define C8_ADDR_PC     (0x0200) // Starting address for program counter
#define C8_ADDR_FONT   (0x0050) // Starting address for system font


// --- call stack -------------------------------------------------------------

#define C8_CALLSTACK_SIZE (32)

// --- cycles -----------------------------------------------------------------

#define C8_OPS_PER_CYCLE (8)

#endif /* SPECIFICATIONS_H */
