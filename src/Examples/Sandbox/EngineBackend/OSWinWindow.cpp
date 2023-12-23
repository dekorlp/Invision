#include "EWindowStatus.h"
#include "EngineCore.h"
#include "OsWinWindow.h"

#if defined(_WIN32)

    LRESULT CALLBACK OSWinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        OSWinWindow* myWnd;
        myWnd = (OSWinWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        switch (msg)
        {
        case WM_CREATE:
        {
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            OSWinWindow* myWnd = (OSWinWindow*)pcs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)myWnd);
            break;
        }
        case WM_SIZE:
        {
            unsigned int clientWidth = 0, clientHeight = 0;
            myWnd->GetWindowClientSize(clientWidth, clientHeight);
            myWnd->mEngineCore->Resize(clientWidth, clientHeight);


            break;
        }

        case WM_QUIT:
	    case WM_DESTROY:
        case WM_CLOSE:
            myWnd->mEngineCore->Shutdown();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }

    WPARAM OSWinWindow::createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow, EngineCore *core, int width, int height, wchar_t* windowname)
    {
        mEngineCore = core;
        WNDCLASSEX wc;
        HWND hwnd;

        //Step 1: Registering the Window Class
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = 0;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = this->mClassName;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc))
        {
            MessageBox(NULL, L"Window Registration Failed!", L"Error!",
                MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }

        // Step 2: Creating the Window
        hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            this->mClassName,
            windowname,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, hInstance, this);
        mHwnd = hwnd;



        if (hwnd == NULL)
        {
            MessageBox(NULL, L"Window Creation Failed!", L"Error!",
                MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }

        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);

        unsigned int clientWidth = 0, clientHeight = 0;
        this->GetWindowClientSize(clientWidth, clientHeight);
        mEngineCore->Create(mHwnd, clientWidth, clientHeight);

        MSG msg;
        bool done = false;

        while (!done)
        {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
			    {
				    done = true;
			    }
            }
		    else
		    {
                unsigned int clientWidth = 0, clientHeight = 0;
                GetWindowClientSize(clientWidth, clientHeight);
                mEngineCore->Render(clientWidth, clientHeight);
		    }
        }

        DestroyWindow(hwnd);

        return msg.wParam;
    }

    void OSWinWindow::GetWindowSize(unsigned int& width, unsigned int& height)
    {
        RECT rect;
        if (!GetWindowRect(mHwnd, &rect)) return;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    void OSWinWindow::GetWindowClientSize(unsigned int& width, unsigned int& height)
    {
        RECT rect;
        if (!GetClientRect(mHwnd, &rect)) return;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }


    OSWinWindow::OSWinWindow()
    {
        mClassName = L"myWindowClass";
        mEngineCore = NULL;
        mHwnd = NULL;
    }

    EWindowStatus OSWinWindow::getWindowStatus()
    {
        EWindowStatus windowStatus;

        if (!IsWindowVisible(mHwnd))
            windowStatus = EWindowStatus::HIDDEN;
        else
            if (IsIconic(mHwnd))
                windowStatus = EWindowStatus::MINIMIZED;
            else
                if (IsZoomed(mHwnd))
                    windowStatus = EWindowStatus::MAXIMIZED;
                else
                {
                    // not hidden, minimized or zoomed, so we are a normal visible window
                    // last ShowWindow flag could have been SW_RESTORE, SW_SHOW, SW_SHOWNA, etc
                    // no way to tell
                    windowStatus = EWindowStatus::SHOWED;
                }

        return windowStatus;
    }

    void OSWinWindow::setWindowTitle(const char* title)
    {
        SetWindowTextA(mHwnd, title);
    }

#elif defined(__linux__)

#endif