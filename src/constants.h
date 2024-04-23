#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

#define SCALE 16

#define SCREEN_BUFFER_WIDTH 64
#define SCREEN_BUFFER_HEIGHT 32

#define REFRESH_RATE 60

#define STACK_SIZE 64
#define MEMORY_SIZE (4 * 1024)

#define NUM_REGISTERS 16

#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

#endif /* CONSTANTS_H */