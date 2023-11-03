#include "IGame.h"
#include "OSWindow.h"

#if defined(_WIN32)

LRESULT CALLBACK OSWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    OSWindow* myWnd;
    myWnd = (OSWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg)
    {
    case WM_CREATE:
    {
        //CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
        //OSWindow* myWnd = (OSWindow*)pcs->lpCreateParams;
        //SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)myWnd);
        myWnd->Tesfunc();
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

WPARAM OSWindow::createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow, IGame* game)
{
    WindowContext* windowContext = new WindowContext();
    GameContext* gameContext = new GameContext();

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

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
        L"AppName",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 360,
        NULL, NULL, hInstance, this);

    windowContext->setHWND(hwnd);
    game->init(*windowContext , *gameContext);


    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    delete gameContext;
    return Msg.wParam;
}

void OSWindow::setHWND(HWND hwnd)
{
    mHwnd = hwnd;
}

HWND OSWindow::getHWND()
{
    return mHwnd;
}

void OSWindow::setWindowName(wchar_t* windowname)
{
    SetWindowTextW(mHwnd, windowname);
}

void OSWindow::setWindowSize(int width, int height)
{
    SetWindowPos(mHwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
}

#elif defined(__linux__)

#endif