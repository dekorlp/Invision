#ifndef ENGINE_H
#define ENGINE_H

#if defined(_WIN32)
#include "EngineCore.h"
#include <windows.h>
#include "Window.h"

#define INVISION_MAIN(IEngineParam) int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, \
_In_ PSTR szCmdLine, _In_ int iCmdShow) { \
    \
    Engine *engine = new IEngineParam(); \
    WPARAM winret = engine->InitWindow(hInstance, hPrevInstance, szCmdLine, iCmdShow); \
\
    return (int)winret; \
} \

#elif defined(__linux__)
#elif defined(__APPLE__)
#endif

class Engine
{
private:    
    EngineCore* mEngineCore;
    IWindow* mWindow;
public:
    Engine()
    {
        mEngineCore = new EngineCore();
    }

    WPARAM InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow) {
        mWindow = new OSWinWindow();
        return dynamic_cast<OSWinWindow*>(mWindow)->createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mEngineCore);
    }

    Window *getWindow() {
		return dynamic_cast<Window*>(mWindow);
	}

    virtual void init() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void destroy() = 0;
};

#endif // ENGINE_H