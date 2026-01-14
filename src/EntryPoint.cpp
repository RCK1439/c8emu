#include "Client/Client.hpp"
#include "Core/Platform.hpp"

#if defined(C8_PLATFORM_WINDOWS) && defined(C8_RELEASE)
#include <Windows.h>

int WINAPI WinMain(UNUSED HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, UNUSED LPSTR lpCmdLine, UNUSED int nShowCmd)
{
    c8emu::Client client(__argc, __argv);
    client.Run();
    return 0;
}

#else
int main(int argc, char** argv)
{
    c8emu::Client client(argc, argv);
    client.Run();
}
#endif
