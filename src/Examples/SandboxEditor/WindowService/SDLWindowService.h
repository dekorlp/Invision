#ifndef SDL_WINDOW_SERVICE_H
#define SDL_WINDOW_SERVICE_H
#include <SDL3/SDL_video.h>

#include "IWindowService.h"
#include "Examples/SandboxEditor/ServiceLocator/ServiceLocator.h"

class SDLWindowService : public IWindowService
{
private:
	SDL_Window* window = nullptr;

public:
	explicit SDLWindowService(ServiceLocator& loc)
		: IWindowService(loc)
	{
	}

	int InitWindow(const char* title, int width, int height) override;
	void* getHandle() override;
	int Destroy() override;
};

#endif