#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "OSWinWindow.h"

#include "IEngine.h"

class EngineCore
{
private:
	IEngine* mEngine = nullptr;
public:
	EngineCore(IEngine* engineInstance)
	{
		mEngine = engineInstance;
	}

	void Create() {
		int test = 0;
		mEngine->init();
	}

	void Render() {
		int test = 0;
		mEngine->render();
		mEngine->update();
	}

	void Resize(unsigned int width, unsigned int height) {

		int test = 0;
	}

	void Shutdown() {
		int test = 0;
		mEngine->destroy();
	}

};

#endif // ENGINE_CORE_H