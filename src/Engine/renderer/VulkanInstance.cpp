#include "precompiled.h"

#include "VulkanRender.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanPushConstant.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanTexture.h"

#include "VulkanEngine.h"

#include "VulkanInstance.h"
namespace Invision
{

	/*VulkanInstance::VulkanInstance(VulkanEngine* engine)
		: IGraphicsInstance(engine)
	{
		vulkanEngine = engine;

	}*/

	VulkanInstance::VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest)
		: IGraphicsInstance(engine)
	{
		mVulkanEngine = engine;
		
		Invision::CreateSurface(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.hwnd);
		Invision::CreatePresentationSystem(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.width, dimensions.height);

		// Create Depth Ressources
		if (activateDepthTest)
		{
			mDepthRessources.CreateDepthRessources(engine->GetVulkanBaseStruct(), engine->GetCommandPool(), engine->GetMemoryManager(), mVulkanContext);
			mUseDepthTest = true;
			mVulkanContext.mUseDepthRessources = true;
		}

		// Create Color Ressources for Multisampling
		if (engine->GetVulkanBaseStruct().UseMSAA)
		{
			mColorRessources.CreateColorRessources(engine->GetVulkanBaseStruct(), engine->GetCommandPool(), engine->GetMemoryManager(), mVulkanContext);
		}

		// Create Default RenderPass / FrameBuffer / CommandBuffer
		mMainRenderPass = CreateRenderPass();
		dynamic_pointer_cast<VulkanRenderPass>(mMainRenderPass)->CreateMainRenderPass(); // create main renderpass

		mMainFramebuffer = CreateFramebuffer(mMainRenderPass);
		dynamic_pointer_cast<VulkanFramebuffer>(mMainFramebuffer)->CreateMainFramebuffer(mMainRenderPass);
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		renderPass = mMainRenderPass;
		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;
	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer )
	{
		Invision::DestroyPresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext);
		Invision::CreatePresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext, canvas.width, canvas.height);

		if (mVulkanEngine->GetVulkanBaseStruct().UseMSAA == true)
		{
			mColorRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
			mColorRessources.CreateColorRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
		}

		if (mUseDepthTest)
		{
			mDepthRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
			mDepthRessources.CreateDepthRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
			mVulkanContext.mUseDepthRessources = true;
		}

		mMainFramebuffer.reset();
		mMainFramebuffer = CreateFramebuffer(mMainRenderPass);
		dynamic_pointer_cast<VulkanFramebuffer>(mMainFramebuffer)->CreateMainFramebuffer(mMainRenderPass);

		// setup commandBuffers
		mMainCommandBuffer.reset();
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;
	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest)
	{
		Invision::DestroyPresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext);
		Invision::CreatePresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext, canvas.width, canvas.height);

		if (mVulkanEngine->GetVulkanBaseStruct().UseMSAA == true)
		{
			mColorRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
			mColorRessources.CreateColorRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
		}

		if (activateDepthTest)
		{
			mDepthRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
			mDepthRessources.CreateDepthRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
			mUseDepthTest = true;
			mVulkanContext.mUseDepthRessources = true;
		}

		mMainFramebuffer.reset();
		mMainFramebuffer = CreateFramebuffer(mMainRenderPass);
		dynamic_pointer_cast<VulkanFramebuffer>(mMainFramebuffer)->CreateMainFramebuffer(mMainRenderPass);

		// setup commandBuffers
		mMainCommandBuffer.reset();
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;
	}

	SVulkanContext& VulkanInstance::GetVulkanContext()
	{
		return mVulkanContext;
	}

	std::shared_ptr <IRenderer> VulkanInstance::CreateRenderer()
	{
		return  std::make_shared<VulkanRenderer >(this);
	}

	std::shared_ptr<IRenderPass> VulkanInstance::CreateRenderPass()
	{
		return  std::make_shared<VulkanRenderPass>(this);
	}

	std::shared_ptr<IVertexBuffer> VulkanInstance::CreateVertexBuffer()
	{
		return  std::make_shared<VulkanVertexBuffer>(this);
	}

	std::shared_ptr<IUniformBuffer> VulkanInstance::CreateUniformBuffer()
	{
		return std::make_shared<VulkanUniformBuffer>(this);
	}

	std::shared_ptr<IPushConstant> VulkanInstance::CreatePushConstant(ShaderStageFlag shaderStages, uint32_t offset, uint32_t size)
	{
		return std::make_shared<VulkanPushConstant>(this, shaderStages, offset, size);
	}

	std::shared_ptr<IIndexBuffer> VulkanInstance::CreateIndexBuffer()
	{
		return std::make_shared<VulkanIndexBuffer>(this);
	}

	std::shared_ptr<IPipeline> VulkanInstance::CreatePipeline()
	{
		return std::make_shared<VulkanPipeline>(this);
	}

	std::shared_ptr<IPipeline> VulkanInstance::CreatePipeline(PipelineProperties* pipelineProperties)
	{
		return std::make_shared<VulkanPipeline>(this, pipelineProperties);
	}

	std::shared_ptr<IFramebuffer> VulkanInstance::CreateFramebuffer(std::shared_ptr<IRenderPass> renderPass)
	{
		return std::make_shared<VulkanFramebuffer>(this, renderPass);
	}

	std::shared_ptr<ICommandBuffer> VulkanInstance::CreateCommandBuffer(std::shared_ptr<Invision::IFramebuffer> framebuffer)
	{
		return std::make_shared<VulkanCommandBuffer>(this, framebuffer);
	}

	std::shared_ptr<ITexture> VulkanInstance::CreateTexture()
	{
		return std::make_shared<VulkanTexture>(this);
	}

	VulkanEngine* VulkanInstance::GetCoreEngine()
	{
		return mVulkanEngine;
	}

	VulkanBaseTexture VulkanInstance::GetDepthRessources()
	{
		return mDepthRessources;
	}

	VulkanBaseTexture VulkanInstance::GetColorRessources()
	{
		return mColorRessources;
	}

	unsigned int VulkanInstance::GetSizeSwapchainImages()
	{
		return static_cast<unsigned int>(mVulkanContext.swapChainImages.size());
	}

	VulkanInstance::~VulkanInstance()
	{
		if (mVulkanEngine->GetVulkanBaseStruct().UseMSAA == true)
		{
			mColorRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
		}

		if (mUseDepthTest)
		{
			mDepthRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
		}
		Invision::DestroyPresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext);
		Invision::DestroySurface(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext);
	}
}