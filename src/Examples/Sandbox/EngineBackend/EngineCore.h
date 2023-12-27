#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <vector>

#include "OSWinWindow.h"

#include "InCommon.h"
#include "renderer/GraphicsFactory.h"
#include "IEngine.h"

class IRenderable;

class EngineCore
{
private:
	IEngine* mEngine = nullptr;
	std::vector<IRenderable*> mRenderables;
	HWND mHwnd = nullptr;
	bool mIsInit = false;

	std::shared_ptr <Invision::IGraphicsEngine> *mGraphicsEngine;
	std::shared_ptr <Invision::IGraphicsInstance> mGraphicsInstance;
	std::shared_ptr <Invision::IRenderPass> mRenderPass;
	std::shared_ptr <Invision::IFramebuffer> mFramebuffer;
	std::shared_ptr <Invision::ICommandBuffer> mCommandBuffer;
	std::shared_ptr <Invision::IRenderer> mRenderer;
public:
	EngineCore(IEngine* engineInstance, std::shared_ptr <Invision::IGraphicsEngine>* graphicsEngine);
#if defined(_WIN32)
	void Create(HWND hwnd, unsigned int width, unsigned int height);
#elif defined(__linux__)

#endif
	void Render(unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void BuildCommandBuffer(unsigned int width, unsigned int height);
	void RecreateSwapchain(unsigned int width, unsigned int height);
	void Shutdown();
	void AddRenderable(IRenderable* mesh);

};

#endif // ENGINE_CORE_H