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
	void Create();
	void Render();
	void Resize(unsigned int width, unsigned int height);
	void Shutdown();

};

#endif // ENGINE_CORE_H