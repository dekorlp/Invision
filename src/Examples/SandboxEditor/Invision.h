#ifndef INVISION_H
#define INVISION_H

#include "RenderService.h"
#include "ServiceLocator.h"
#include "SDL3/SDL.h"


class Invision
{
private:
	void eventTriggered(SDL_Event event);
	ServiceLocator serviceLocator;
	RenderService *renderService;
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