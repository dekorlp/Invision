#include "Invision.h"
#include "ConfigService/IntValue.h"
#include "ConfigService/StringValue.h"
#include "EventService/SDLEventService.h"
#include "RenderService/VulkanRenderService.h"
#include "WindowService/SDLWindowService.h"

void Invision::Run()
{
    configService = new ConfigService(serviceLocator);
    serviceLocator.RegisterSystem(configService);
    configService->SetConfig("windowTitle", std::make_shared<StringValue>("An SDL3 Window"));
    configService->SetConfig("windowWidth", std::make_shared<IntValue>(640));
    configService->SetConfig("windowHeight", std::make_shared<IntValue>(480));

    entityService = new EntityService(serviceLocator);
    serviceLocator.RegisterSystem(entityService);

    entityService->RegisterComponent<Position>();
    entityService->RegisterComponent<Velocity>();

    Entity e1 = entityService->CreateEntity();
    Entity e2 = entityService->CreateEntity();

    entityService->AddComponent(e1, Position{ 10.0f, 20.0f });
    entityService->AddComponent(e1, Velocity{ 1.0f, 0.5f });

    entityService->AddComponent(e2, Position{ 30.0f, 40.0f });

    // **Komponenten abrufen (Schneller Zugriff O(1))**
    auto& pos1 = entityService->GetComponent<Position>(e1);
    std::cout << "Entity 1 Position: (" << pos1.x << ", " << pos1.y << ")\n";

    // **Komponente entfernen**
    entityService->DestroyEntity(e1);

    windowService = new SDLWindowService(serviceLocator);
    serviceLocator.RegisterSystem(windowService);
    windowService->InitWindow(
        configService->GetConfig<StringValue>("windowTitle")->get().c_str(), 
        configService->GetConfig<IntValue>("windowWidth")->get(), 
        configService->GetConfig<IntValue>("windowHeight")->get()
    );
    void* handle = windowService->getHandle();

    renderService = new VulkanRenderService(serviceLocator);
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