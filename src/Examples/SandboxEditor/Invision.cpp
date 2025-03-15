#include <SDL3/SDL.h>
#include "Invision.h"

#include "ConfigService/IntValue.h"
#include "ConfigService/StringValue.h"
#include "EventService/SDLEventService.h"
#include "WindowService/SDLWindowService.h"

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

    windowService = new SDLWindowService(serviceLocator);
    serviceLocator.RegisterSystem(windowService);
    windowService->InitWindow(
        configService->GetConfig<StringValue>("windowTitle")->get().c_str(), 
        configService->GetConfig<IntValue>("windowWidth")->get(), 
        configService->GetConfig<IntValue>("windowHeight")->get()
    );
    void* handle = windowService->getHandle();

    renderService = new RenderService(serviceLocator);
    serviceLocator.RegisterSystem(renderService);
    renderService->Render();

    eventService = new SDLEventService(serviceLocator);
    serviceLocator.RegisterSystem(eventService);
    eventService->InitEventLoop();

    windowService->Destroy();
}

Invision::~Invision()
{
    delete renderService;
    delete configService;
    delete eventService;
    delete windowService;
}