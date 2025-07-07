#include "client/client.h"

int main(int argc, char **argv)
{
    Chip8Client *const client = c8InitClient(argc, argv);
    if (!client)
    {
        return C8_FAILURE;
    }

    c8RunClient(client);
    c8CloseClient(client);

    return C8_SUCCESS;
}
