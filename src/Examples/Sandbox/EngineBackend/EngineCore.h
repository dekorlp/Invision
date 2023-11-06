#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "OSWinWindow.h"

#include "IEngine.h"

class EngineCore
{
private:
	IEngine* mEngine = nullptr;
public:
	EngineCore(IEngine* engineInstance);
#if defined(_WIN32)
	void Create(HWND hwnd, unsigned int width, unsigned int height);
#elif defined(__linux__)

#endif
	void Render(unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Shutdown();

};

#endif // ENGINE_CORE_H