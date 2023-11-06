#include "IEngine.h"
#include "Engine.h"

Engine::Engine()
{
    mWindow = new Window();
    mEngineCore = new EngineCore(this);
}

#if defined(_WIN32)
WPARAM Engine::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    return dynamic_cast<OSWinWindow*>(mWindow)->createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mEngineCore, 640, 360, L"Demo App");
}
#elif defined(__linux__)

#endif

Window* Engine::getWindow() {
    return dynamic_cast<Window*>(mWindow);
}

Engine::~Engine()
{
    delete mWindow;
	delete mEngineCore;
}