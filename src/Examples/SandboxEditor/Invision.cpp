#include <SDL3/SDL.h>

#include <functional>
#include "Window.h"
#include "Invision.h"

#include "ConfigService/IntValue.h"
#include "ConfigService/StringValue.h"
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
    configService = new ConfigService(serviceLocator);
    serviceLocator.RegisterSystem(configService);

    configService->SetConfig("windowTitle", std::make_shared<StringValue>("An SDL3 Window"));
    configService->SetConfig("windowWidth", std::make_shared<IntValue>(640));
    configService->SetConfig("windowHeight", std::make_shared<IntValue>(480));

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