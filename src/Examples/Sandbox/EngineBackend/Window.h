#ifndef WINDOW_H
#define WINDOW_H

class OSWinWindow;
class IWindow;

class Window : protected OSWinWindow
{
public:
    bool isWindowActive() override
    {
        return OSWinWindow::isWindowActive();
    }
};

#endif WINDOW_H