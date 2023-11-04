#ifndef WINDOW_H
#define WINDOW_H

enum EWindowStatus;
class OSWinWindow;

class Window : public OSWinWindow
{
public:
    Window();

    EWindowStatus getWindowStatus();

    WPARAM createWindow() = delete;
};

#endif WINDOW_H