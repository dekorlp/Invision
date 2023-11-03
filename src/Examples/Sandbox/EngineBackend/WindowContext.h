#ifndef WINDOW_CONTEXT_H
#define WINDOW_CONTEXT_H

#include "OSWindow.h"

class WindowContext : public OSWindow
{
public:
	WindowContext() : OSWindow() {};

	void setWindowName(wchar_t* windowname);
	wchar_t* getWindowName();

	void setWindowSize(int width, int height);
	int getWindowWidth();
	int getWindowHeight();

private:
	wchar_t* mWindowname;
	int mWindowWidth;
	int mWindowHeight;
};

#endif // WINDOW_CONTEXT_H