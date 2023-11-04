#include "EngineCore.h"

EngineCore::EngineCore(IEngine* engineInstance)
{
	mEngine = engineInstance;
}

void EngineCore::Create() {
	int test = 0;
	mEngine->init();
}

void EngineCore::Render() {
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