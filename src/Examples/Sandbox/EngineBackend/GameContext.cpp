#include "GameContext.h"

void GameContext::setWindowName(wchar_t* windowname)
{
	mWindowname = windowname;
	SetWindowTextW(mHwnd, mWindowname);
}
wchar_t* GameContext::getWindowName()
{
	return mWindowname;
}

void GameContext::setWindowSize(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	SetWindowPos(mHwnd, HWND_TOP, 0, 0, mWindowWidth, mWindowHeight, SWP_NOMOVE);
}

int GameContext::getWindowWidth()
{
	return mWindowWidth;
}

int GameContext::getWindowHeight()
{
	return mWindowHeight;
}

void GameContext::setHWND(HWND hwnd)
{
	mHwnd = hwnd;
}

HWND GameContext::getHWND()
{
	return mHwnd;
}

//void GameContext::setWindowContext(WindowContext* windowContext)
//{
//	mWindowContext = windowContext;
//}