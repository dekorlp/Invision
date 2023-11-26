#include "Mesh.h"
#include "EngineCore.h"

EngineCore::EngineCore(IEngine* engineInstance, std::shared_ptr <Invision::IGraphicsEngine> *graphicsEngine)
{
	mEngine = engineInstance;
	mGraphicsEngine = graphicsEngine;
}

#if defined(_WIN32)
void EngineCore::Create(HWND hwnd, unsigned int width, unsigned int height) {
	int test = 0;
	mEngine->init();
	Invision::CanvasDimensions dim = { hwnd, width, height };

	try
	{
		mGraphicsInstance = (*mGraphicsEngine)->CreateInstance(dim, mRenderPass, mFramebuffer, mCommandBuffer);
	}
	catch (Invision::InvisionNotEnoughDeviceOrHostMemory iEx)
	{
		std::stringstream ss;
		ss << iEx.what();
		ss << "\n";
		INVISION_LOG_INFO(ss.str());
	}
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

void EngineCore::AddMesh(Mesh* mesh) 
{
	mMeshes.push_back(mesh);
}