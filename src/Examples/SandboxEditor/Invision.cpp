#include <SDL3/SDL.h>

#include <functional>
#include "Window.h"
#include "Invision.h"
void Invision::eventTriggered(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
        break;
    case SDL_EVENT_WINDOW_MINIMIZED:
        break;
    case SDL_EVENT_KEY_UP:
        int test2 = 0;
        break;
    }
}

void Invision::Run()
{
    Window* win = new Window;
    win->InitWindow("An SDL3 window", 640, 480);

    renderService = new RenderService(serviceLocator);

    serviceLocator.RegisterSystem(renderService);
    renderService->Render();
    //RenderService serv* = serviceLocator.getService<RenderService>();

    void* handle = win->getHandle();

    win->InitEventLoop(std::bind(&Invision::eventTriggered, this, std::placeholders::_1));

    win->Destroy();
    delete win;
}

Invision::~Invision()
{
    delete renderService;
}