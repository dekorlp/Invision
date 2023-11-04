#include "IEngine.h"
#include "Engine.h"

Engine::Engine()
{
    mWindow = new Window();
    mEngineCore = new EngineCore(this);
}

WPARAM Engine::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    return dynamic_cast<OSWinWindow*>(mWindow)->createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mEngineCore);
}

Window* Engine::getWindow() {
    return dynamic_cast<Window*>(mWindow);
}

Engine::~Engine()
{
    delete mWindow;
	delete mEngineCore;
}