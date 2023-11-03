#ifndef WINDOW_CONTEXT_H
#define WINDOW_CONTEXT_H

#include "OSWindow.h"

class WindowContext : public OSWindow
{
public:
	WindowContext() : OSWindow()
	{
		
	}

private:
	HWND mHwnd;
};

#endif // WINDOW_CONTEXT_H