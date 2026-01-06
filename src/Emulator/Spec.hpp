#pragma once

#include "Core/Types.hpp"

namespace c8emu {

// --- general ----------------------------------------------------------------

constexpr size_t C8_NUM_KEYS = 16;

// --- cpu --------------------------------------------------------------------

constexpr size_t C8_NUM_REGISTERS = 16;

constexpr size_t C8_SCREEN_BUFFER_WIDTH  = 64;
constexpr size_t C8_SCREEN_BUFFER_HEIGHT = 32;

// --- memory -----------------------------------------------------------------

constexpr Address C8_ADDR_ROM    = 0x0200; // Starting address for ROM data
constexpr Address C8_ADDR_SCREEN = 0x0F00; // Starting address for Screen
constexpr Address C8_ADDR_STACK  = 0x0FA0; // Starting address for call stack
constexpr Address C8_ADDR_PC     = 0x0200; // Starting address for program counter
constexpr Address C8_ADDR_FONT   = 0x0050; // Starting address for system font

constexpr size_t C8_FONTSET_SIZE = 80;
constexpr size_t C8_MEMORY_SIZE  = 4 * 1024;
constexpr size_t C8_MAX_ROM_SIZE = C8_MEMORY_SIZE - C8_ADDR_ROM;

// --- call stack -------------------------------------------------------------

constexpr size_t C8_CALLSTACK_SIZE = 32;

// --- cycles -----------------------------------------------------------------

constexpr u8 C8_OPS_PER_CYCLE = 8;
constexpr float C8_TICK_RATE  = 1.0f / 60.0f;

}
