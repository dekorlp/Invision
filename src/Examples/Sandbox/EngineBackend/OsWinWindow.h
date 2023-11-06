#ifndef OS_WINWINDOW_H
#define OS_WINWINDOW_H

#if defined(_WIN32)
#include "IWindow.h"
#include <windows.h>

class EngineCore;
enum EWIndowStatus;

class OSWinWindow : public IWindow
{
private:
    wchar_t* mClassName;
    EngineCore* mEngineCore;
    HWND mHwnd;

    static LRESULT CALLBACK OSWinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    OSWinWindow();
    WPARAM createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow, EngineCore* core, int width, int height, wchar_t* windowname);

protected:
    virtual EWindowStatus getWindowStatus();
    virtual void setWindowTitle(const char* title);
};

#elif defined(__linux__)

#endif

#endif //OS_WINDOW_H