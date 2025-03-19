#ifndef INVISION_H
#define INVISION_H

#include "ConfigService/ConfigService.h"
#include "EntityService/EntityService.h"
#include "EventService/IEventService.h"
#include "RenderService/RenderService.h"
#include "ServiceLocator/ServiceLocator.h"
#include "WindowService/IWindowService.h"

struct Position {
	float x, y;
};

struct Velocity {
	float vx, vy;
};

class Invision
{
private:
	ServiceLocator serviceLocator;
	EntityService* entityService = nullptr;
	ConfigService * configService = nullptr;
	RenderService * renderService = nullptr;
	IWindowService* windowService = nullptr;
	IEventService* eventService = nullptr;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;

	void Run();

	~Invision();
};

#endif //INVISION_H

#ifdef _WIN32
#define INVISION_MAIN(InvisionGameClass) \
	void main() { \
		Invision *gameInstance = new InvisionGameClass(); \
		gameInstance->Run(); \
		delete gameInstance; \
	 } \

#endif