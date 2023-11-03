#ifndef OS_WINDOW_H
#define OS_WINDOW_H

#if defined(_WIN32)
#include <windows.h>

class IGame;

class OSWindow
{
private:
    wchar_t* mClassName;

protected:
    HWND mHwnd;
    void setWindowName(wchar_t* windowname);
    void setWindowSize(int width, int height);

    static LRESULT CALLBACK OSWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

    OSWindow()
    {
        mClassName = L"myWindowClass";
    }

    WPARAM createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow, IGame* game);
};

#elif defined(__linux__)

#endif

#endif //OS_WINDOW_H