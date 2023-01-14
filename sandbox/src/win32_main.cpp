#include <Windows.h>

#include <renderer/platform/window/Window.h>

int CALLBACK 
WinMain([[maybe_unused]] HINSTANCE hInstance, 
        [[maybe_unused]] HINSTANCE hPrevInstance,
        [[maybe_unused]] LPSTR     lpCmdLine,
        [[maybe_unused]] int       nShowCmd)
{
    PGM::Platform::Window wnd{"PGM - Sandbox"};
    wnd.show();

    return 0;
}