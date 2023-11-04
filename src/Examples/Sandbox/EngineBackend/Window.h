#ifndef WINDOW_H
#define WINDOW_H

class OSWinWindow;
class IWindow;

class Window : public OSWinWindow
{
public:
    Window() : OSWinWindow() {

    }

    bool isWindowActive()
    {
        return OSWinWindow::isWindowActive();
    }

    WPARAM createWindow() = delete;
};

#endif WINDOW_H