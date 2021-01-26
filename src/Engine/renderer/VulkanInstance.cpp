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

	VulkanInstance::VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest, MSAAMode msaa)
		: IGraphicsInstance(engine)
	{
		vulkanEngine = engine;
		
		Invision::CreateSurface(engine->GetVulkanInstance(), vulkanContext, dimensions.hwnd);
		Invision::CreatePresentationSystem(engine->GetVulkanInstance(), vulkanContext, dimensions.width, dimensions.height);
		if (activateDepthTest)
		{
			depthRessources.CreateDepthRessources(engine->GetVulkanInstance(), engine->GetCommandPool(), vulkanContext);
			mUseDepthTest = true;
		}

		// Create Default RenderPass / FrameBuffer / CommandBuffer
		mMainRenderPass = CreateRenderPass();
		dynamic_pointer_cast<VulkanRenderPass>(mMainRenderPass)->CreateMainRenderPass(); // create main renderpass

		mMainFramebuffer = CreateFramebuffer(mMainRenderPass, static_cast<unsigned int>( vulkanContext.swapChainImageViews.size()));
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		renderPass = mMainRenderPass;
		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;

		switch (msaa)
		{
			case MSAAMODE_OFF:
				vulkanContext.UseMSAA = false;
				break;
			case MSAAMODE_SAMPLE_COUNT_1:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_1_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_2:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_2_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_4:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_4_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_8:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_8_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_16:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_16_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_32:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_32_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_64:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(engine->GetVulkanInstance(), VK_SAMPLE_COUNT_64_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_BEST:
				vulkanContext.UseMSAA = true;
				vulkanContext.MsaaFlagBits = Invision::GetMaxUsableSampleCount(engine->GetVulkanInstance());
				break;

		}

	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer )
	{
		Invision::DestroyPresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext);
		Invision::CreatePresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext, canvas.width, canvas.height);

		if (mUseDepthTest)
		{
			depthRessources.DestroyDepthRessources(vulkanEngine->GetVulkanInstance());
			depthRessources.CreateDepthRessources(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), vulkanContext);
		}

		mMainFramebuffer.reset();
		mMainFramebuffer = CreateFramebuffer(mMainRenderPass, static_cast<unsigned int>(vulkanContext.swapChainImageViews.size()));

		// setup commandBuffers
		mMainCommandBuffer.reset();
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;
	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest)
	{
		Invision::DestroyPresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext);
		Invision::CreatePresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext, canvas.width, canvas.height);

		if (activateDepthTest)
		{
			depthRessources.DestroyDepthRessources(vulkanEngine->GetVulkanInstance());
			depthRessources.CreateDepthRessources(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), vulkanContext);
			mUseDepthTest = true;
		}

		mMainFramebuffer.reset();
		mMainFramebuffer = CreateFramebuffer(mMainRenderPass, static_cast<unsigned int>(vulkanContext.swapChainImageViews.size()));

		// setup commandBuffers
		mMainCommandBuffer.reset();
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		framebuffer = mMainFramebuffer;
		commandBuffer = mMainCommandBuffer;
	}

	SVulkanContext& VulkanInstance::GetVulkanContext()
	{
		return vulkanContext;
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

	std::shared_ptr<IFramebuffer> VulkanInstance::CreateFramebuffer(std::shared_ptr<IRenderPass> renderPass, unsigned int countFrameBuffers)
	{
		return std::make_shared<VulkanFramebuffer>(this, renderPass, countFrameBuffers);
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
		return vulkanEngine;
	}

	VulkanBaseDepthRessources VulkanInstance::GetDepthRessources()
	{
		return depthRessources;
	}

	unsigned int VulkanInstance::GetSizeSwapchainImages()
	{
		return static_cast<unsigned int>(vulkanContext.swapChainImages.size());
	}

	VulkanInstance::~VulkanInstance()
	{
		if (mUseDepthTest)
		{
			depthRessources.DestroyDepthRessources(vulkanEngine->GetVulkanInstance());
		}
		Invision::DestroyPresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext);
		Invision::DestroySurface(vulkanEngine->GetVulkanInstance(), vulkanContext);
	}
}