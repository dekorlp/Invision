#include "OsWinWindow.h"
#include "IWindow.h"
#include "Window.h"

Window::Window() : OSWinWindow() {

}

bool Window::isWindowActive()
{
    return OSWinWindow::isWindowActive();
}
