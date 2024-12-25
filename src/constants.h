#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_BUFFER_WIDTH 64
#define SCREEN_BUFFER_HEIGHT 32

#define SCALE (WINDOW_WIDTH / SCREEN_BUFFER_WIDTH)

#define MEMORY_SIZE (4 * 1024)
#define FONTSET_SIZE 80

#define NUM_REGISTERS 16
#define NUM_KEYS 16

#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

#endif /* CONSTANTS_H */

