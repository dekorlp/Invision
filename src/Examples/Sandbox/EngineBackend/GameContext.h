#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <wtypes.h>
//#include "WindowContext.h"

class GameContext
{
public:
	void setWindowName(wchar_t* windowname);
	wchar_t* getWindowName();

	void setWindowSize(int width, int height);
	int getWindowWidth();
	int getWindowHeight();

	void setHWND(HWND hwnd);
	HWND getHWND();

	//void setWindowContext(WindowContext windowContext);
private:
	wchar_t* mWindowname;
	int mWindowWidth;
	int mWindowHeight;
	HWND mHwnd;
	//WindowContext mWindowContext;
};

#endif // GAME_CONTEXT_H