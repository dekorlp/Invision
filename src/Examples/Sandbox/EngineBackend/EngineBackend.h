#pragma once

#if defined(_WIN32)
#include <windows.h>
#include "OSWindow.h"

const wchar_t g_szClassName[] = L"myWindowClass";

#define INVISION_MAIN(IGameParam) int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, \
_In_ PSTR szCmdLine, _In_ int iCmdShow) { \
    \
    IGame *game = new IGameParam(); \
	OSWindow *window = new OSWindow();\
    WPARAM winret = window->createWindow(hInstance, hPrevInstance, szCmdLine, iCmdShow, &(*game)); \
\
    return (int)winret; \
} \

#elif defined(__linux__)

#include <X11/Xlib.h>
#include <unistd.h>

main()
{
    // Open a display.
    Display* d = XOpenDisplay(0);

    if (d)
    {
        // Create the window
        Window w = XCreateWindow(d, DefaultRootWindow(d), 0, 0, 200,
            100, 0, CopyFromParent, CopyFromParent,
            CopyFromParent, 0, 0);

        // Show the window
        XMapWindow(d, w);
        XFlush(d);

        // Sleep long enough to see the window.
        sleep(10);
    }
    return 0;
}

#elif defined(__APPLE__)


#endif