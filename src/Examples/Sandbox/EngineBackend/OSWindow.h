#ifndef OS_WINDOW_H
#define OS_WINDOW_H

#if defined(_WIN32)
#include <windows.h>
#include "IGame.h"

class OSWindow
{
private:
    wchar_t* mClassName;

public:
    OSWindow()
    {
        mClassName = L"myWindowClass";
    }

    static LRESULT CALLBACK OSWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WPARAM createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow, IGame* game);

    void Tesfunc() {
        int msgboxID = MessageBox(
            NULL,
            (LPCWSTR)L"Das ist eine Testmsgbox",
            (LPCWSTR)L"Account Details",
            MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
        );
    }	
};

#elif defined(__linux__)

#endif

#endif //OS_WINDOW_H