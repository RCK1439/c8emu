#ifndef CLIENT_H
#define CLIENT_H

#include "core/types.h"

typedef struct Chip8Client Chip8Client;

Chip8Client *c8InitClient(i32 argc, char **argv);
void c8RunClient(Chip8Client *client);
void c8CloseClient(Chip8Client *client);

#endif /* CLIENT_H */
