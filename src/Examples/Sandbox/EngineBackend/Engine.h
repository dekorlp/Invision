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

class Engine : public IEngine
{
private:    
    EngineCore* mEngineCore;
    IWindow* mWindow;
public:
    Engine();

    WPARAM InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
    Window* getWindow();

    
};

#endif // ENGINE_H