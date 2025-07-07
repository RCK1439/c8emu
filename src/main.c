#include "client/client.h"

int main(int argc, char **argv)
{
    Chip8Client *const client = c8InitClient(argc, argv);
    if (!client)
    {
        return 0;
    }

    c8RunClient(client);
    c8CloseClient(client);
}
