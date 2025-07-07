#include "client/client.h"

int main(int argc, char **argv)
{
    Chip8Client *const client = c8InitClient(argc, argv);
    c8RunClient(client);
    c8CloseClient(client);
}
