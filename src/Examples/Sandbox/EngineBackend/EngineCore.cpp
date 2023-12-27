#include "IRenderable.h"
#include "Mesh.h"
#include "Shape.h"
#include "EngineCore.h"

EngineCore::EngineCore(IEngine* engineInstance, std::shared_ptr <Invision::IGraphicsEngine> *graphicsEngine)
{
	mEngine = engineInstance;
	mGraphicsEngine = graphicsEngine;
}

#if defined(_WIN32)
void EngineCore::Create(HWND hwnd, unsigned int width, unsigned int height) {
	mEngine->init();
	Invision::CanvasDimensions dim = { hwnd, width, height };
	mHwnd = hwnd;

	try
	{
		mGraphicsInstance = (*mGraphicsEngine)->CreateInstance(dim, mRenderPass, mFramebuffer, mCommandBuffer);

		for(unsigned int i = 0; i < mRenderables.size(); i++)
		{
			mRenderables[i]->Initialize(mGraphicsInstance, mRenderPass);
		}

		this->BuildCommandBuffer(width, height);
		mRenderer = mGraphicsInstance->CreateRenderer();
		mIsInit = true;
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

void EngineCore::BuildCommandBuffer(unsigned int width, unsigned int height)
{
	mCommandBuffer->BeginCommandBuffer();
	mCommandBuffer->SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 });
	mCommandBuffer->SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height });
	mCommandBuffer->BeginRenderPass(mRenderPass, mFramebuffer, 0, 0, width, height);
	for (unsigned int i = 0; i < mRenderables.size(); i++)
	{
		mCommandBuffer->BindPipeline(mRenderables[i]->GetPipeline());
		mCommandBuffer->BindVertexBuffer({ mRenderables[i]->GetVertexBuffer() }, 0, 1);
		mCommandBuffer->BindDescriptorSets(mRenderables[i]->GetUniformBuffer(), mRenderables[i]->GetPipeline());
		if (mRenderables[i]->HasIndexBuffer())
		{
			mCommandBuffer->BindIndexBuffer(mRenderables[i]->GetIndexBuffer(), Invision::INDEX_TYPE_UINT32);
			if (typeid(mRenderables[i]) == typeid(Mesh))
			{
				mCommandBuffer->DrawIndexed(static_cast<uint32_t>(dynamic_cast<Mesh*>(mRenderables[i])->GetIndices().size()), 1, 0, 0, 0);
			}
			else
			{
				mCommandBuffer->DrawIndexed(static_cast<uint32_t>(dynamic_cast<Shape*>(mRenderables[i])->GetIndices().size()), 1, 0, 0, 0);
			}
		}
		else
		{
			if (typeid(mRenderables[i]) == typeid(Mesh))
			{
				mCommandBuffer->Draw(static_cast<uint32_t>(dynamic_cast<Mesh*>(mRenderables[i])->GetVertexCount()), 1, 0, 0);
			}
			else
			{
				mCommandBuffer->Draw(static_cast<uint32_t>(dynamic_cast<Shape*>(mRenderables[i])->GetVertexCount()), 1, 0, 0);
			}
		}

		
	}

	mCommandBuffer->EndRenderPass();
	mCommandBuffer->EndCommandBuffer();
}

void EngineCore::RecreateSwapchain(unsigned int width, unsigned int height)
{
	mGraphicsInstance->ResetPresentation({ mHwnd, (unsigned int)width, (unsigned int)height }, mRenderPass, mFramebuffer, mCommandBuffer);
	BuildCommandBuffer(width, height);
}

void EngineCore::Render(unsigned int width, unsigned int height) {
	if (mIsInit == false)
		return;

	// my render code
	bool recreateSwapchainIsNecessary = false;
	recreateSwapchainIsNecessary = mRenderer->PrepareFrame();

	mRenderer->Draw(mCommandBuffer);

	recreateSwapchainIsNecessary = mRenderer->SubmitFrame();

	if (recreateSwapchainIsNecessary) this->RecreateSwapchain(width, height);

	mEngine->render();
	mEngine->update();
}

void EngineCore::Resize(unsigned int width, unsigned int height) {

	if ((width < 0) || (height < 0))
		return;

	// my resize Code 
	if (mIsInit == true)
		this->RecreateSwapchain(width, height);
}

void EngineCore::Shutdown() {
	mEngine->destroy();
}

void EngineCore::AddRenderable(IRenderable* mesh)
{
	mRenderables.push_back(mesh);
}