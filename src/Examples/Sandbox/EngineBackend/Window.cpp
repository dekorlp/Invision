#include "EWindowStatus.h"
#include "OsWinWindow.h"
#include "IWindow.h"
#include "Window.h"

Window::Window() : OSWinWindow() {

}

EWindowStatus Window::getWindowStatus()
{
    return OSWinWindow::getWindowStatus();
}

void Window::setWindowTitle(const char* title)
{
    OSWinWindow::setWindowTitle(title);
}