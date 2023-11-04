#ifndef OS_WINWINDOW_H
#define OS_WINWINDOW_H

#if defined(_WIN32)
#include "IWindow.h"
#include <windows.h>

class OSWinWindow : public IWindow
{
private:
    wchar_t* mClassName;
    HWND mHwnd;

    static LRESULT CALLBACK OSWinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void setHWND(HWND hwnd);
    HWND getHWND();

    void Tesfunc() {
        int msgboxID = MessageBox(
            NULL,
            (LPCWSTR)L"Das ist eine Testmsgbox",
            (LPCWSTR)L"Account Details",
            MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
        );
    }

public:

    OSWinWindow()
    {
        mClassName = L"myWindowClass";
    }

    WPARAM createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow);

    void resize()
    {


    }
};

#elif defined(__linux__)

#endif

#endif //OS_WINDOW_H