#ifndef SDL_EVENT_SERVICE_H
#define SDL_EVENT_SERVICE_H
#include "IEventService.h"
#include "Examples/SandboxEditor/WindowService/IWindowService.h"

class SDLEventService : public IEventService
{
private:
	bool done = false;
public:
	explicit SDLEventService(ServiceLocator& loc)
		: IEventService(loc)
	{
	}

	void InitEventLoop() override;
};

#endif