#include "WindowContext.h"

void WindowContext::setWindowName(wchar_t* windowname)
{
	mWindowname = windowname;
	OSWindow::setWindowName(mWindowname);
}
wchar_t* WindowContext::getWindowName()
{
	return mWindowname;
}

int WindowContext::getWindowHeight()
{
	return mWindowHeight;
}

void WindowContext::setWindowSize(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	OSWindow::setWindowSize(mWindowWidth, mWindowHeight);
}

int WindowContext::getWindowWidth()
{
	return mWindowWidth;
}
