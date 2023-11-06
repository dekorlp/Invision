#include "EngineCore.h"

EngineCore::EngineCore(IEngine* engineInstance)
{
	mEngine = engineInstance;
}

#if defined(_WIN32)
void EngineCore::Create(HWND hwnd, unsigned int width, unsigned int height) {
	int test = 0;
	mEngine->init();
}
#elif defined(__linux__)

#endif
void EngineCore::Render(unsigned int width, unsigned int height) {
	int test = 0;
	mEngine->render();
	mEngine->update();
}

void EngineCore::Resize(unsigned int width, unsigned int height) {

	int test = 0;
}

void EngineCore::Shutdown() {
	int test = 0;
	mEngine->destroy();
}