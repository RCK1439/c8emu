#include "client/client.hpp"
#include "core/platform.hpp"

#if defined(C8_PLATFORM_WINDOWS) && defined(C8_RELEASE)

typedef struct HINSTANCE__ *HINSTANCE;
typedef char *LPSTR;

extern "C" {
_ACRTIMP int*       __cdecl __p___argc (void);
_ACRTIMP char***    __cdecl __p___argv (void);
_ACRTIMP wchar_t*** __cdecl __p___wargv(void);
}

#ifdef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY
    extern int       __argc;
    extern char**    __argv;
    extern wchar_t** __wargv;
#else
    #define __argc  (*__p___argc())  // Pointer to number of command line arguments
    #define __argv  (*__p___argv())  // Pointer to table of narrow command line arguments
    #define __wargv (*__p___wargv()) // Pointer to table of wide command line arguments
#endif

int __stdcall WinMain(UNUSED HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, UNUSED LPSTR lpCmdLine, UNUSED int nShowCmd)
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
