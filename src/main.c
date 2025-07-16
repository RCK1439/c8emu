#include "client/client.h"
#include "core/platform.h"

#if defined(C8_PLATFORM_WINDOWS) && defined(C8_RELEASE)
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    (void)hInstance;     // --+
    (void)hPrevInstance; //   |
    (void)lpCmdLine;     //   +-- unused parameters
    (void)nShowCmd;      // --+

    C8Client *const client = c8InitClient(__argc, __argv);
    if (!client)
    {
        return C8_FAILURE;
    }

    c8RunClient(client);
    c8CloseClient(client);

    return C8_SUCCESS;
}

#else
int main(int argc, char **argv)
{
    C8Client *const client = c8InitClient(argc, argv);
    if (!client)
    {
        return C8_FAILURE;
    }

    c8RunClient(client);
    c8CloseClient(client);

    return C8_SUCCESS;
}
#endif
