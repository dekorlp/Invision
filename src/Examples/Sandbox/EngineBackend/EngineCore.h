#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "OSWinWindow.h"

class EngineCore
{
private:
	IWindow* mWindow;

public:
	WPARAM InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
		mWindow = new OSWinWindow();
		return dynamic_cast<OSWinWindow*>(mWindow)->createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, this);
	}

	void Create() {
		int test = 0;
	}

	void Render() {
		int test = 0;
	}

	void Resize(unsigned int width, unsigned int height) {

		int test = 0;
	}

	void Shutdown() {
		int test = 0;
	}

};

#endif // ENGINE_CORE_H