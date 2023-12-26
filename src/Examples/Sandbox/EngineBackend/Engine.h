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

#include "InCommon.h"
#include "renderer/GraphicsFactory.h"

class IMesh;

class Engine : public IEngine
{
private:    
    EngineCore* mEngineCore;
    IWindow* mWindow;
    Invision::Log log;

    std::shared_ptr <Invision::IGraphicsEngine> mGraphicsEngine;
public:
    Engine();
#if defined(_WIN32)
    WPARAM InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#elif defined(__linux__)

#endif

    Window* getWindow();
    void AddMesh(IMesh* mesh);
    ~Engine();
};

#endif // ENGINE_H