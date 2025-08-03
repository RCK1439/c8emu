#include "client/client.h"
#include "core/platform.h"

#if defined(C8_PLATFORM_WINDOWS) && defined(C8_RELEASE)
#include <windows.h>

int WINAPI WinMain(UNUSED HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, UNUSED LPSTR lpCmdLine, UNUSED int nShowCmd)
{
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
