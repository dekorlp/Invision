#include <SDL3/SDL.h>


#include "Window.h"
#include "Invision.h"

void Invision::Run()
{
    Window* win = new Window;
    win->InitWindow("An SDL3 window", 640, 480);

    void* handle = win->getHandle();

    win->InitEventLoop();

    win->Destroy();
}