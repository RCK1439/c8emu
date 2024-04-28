/**
 * @file   constants.h
 * @brief  Defines constants to be used throughout the emulator
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* --- window details ------------------------------------------------------ */

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

#define SCALE 16

/* --- screen buffer details ----------------------------------------------- */

#define SCREEN_BUFFER_WIDTH 64
#define SCREEN_BUFFER_HEIGHT 32

#define REFRESH_RATE 60

/* --- memory specifications ----------------------------------------------- */

#define STACK_SIZE 32
#define MEMORY_SIZE (4 * 1024)
#define FONTSET_SIZE 80

/* --- register specification ---------------------------------------------- */

#define NUM_REGISTERS 16
#define NUM_KEYS 16

/* --- memory regions ------------------------------------------------------ */

#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

#endif /* CONSTANTS_H */
