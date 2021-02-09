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

	VulkanInstance::VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, bool activateDepthTest, MSAAMode msaaMode)
		: IGraphicsInstance(engine)
	{
		mVulkanEngine = engine;

		ActivateMSAA(msaaMode);
		
		Invision::CreateSurface(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.hwnd);
		Invision::CreatePresentationSystem(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.width, dimensions.height);

		// Create Default RenderPass / FrameBuffer / CommandBuffer
		mMainRenderPass = CreateRenderPass();
		std::vector<Invision::VulkanBaseTexture> frameBufferAttachments;
		// Create Depth Ressources
		if (activateDepthTest)
		{
			UpdateDepthTexture();
		}

		// Create Color Ressources for Multisampling
		if (engine->GetVulkanBaseStruct().UseMSAA)
		{
			UpdateMSAATexture();
			dynamic_pointer_cast<VulkanRenderPass>(mMainRenderPass)->CreateMainRenderPass(mDepthRessources, mColorRessources); // create main renderpass
		}
		else
		{
			dynamic_pointer_cast<VulkanRenderPass>(mMainRenderPass)->CreateMainRenderPass(mDepthRessources); // create main renderpass
		}

	
	

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
			UpdateMSAATexture();
		}

		if (mUseDepthTest)
		{
			UpdateDepthTexture();
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

	void VulkanInstance::ActivateMSAA(MSAAMode msaa)
	{
		if (mVulkanEngine->GetVulkanBaseStruct().physicalDeviceStruct.deviceFeatures.sampleRateShading == VK_TRUE)
		{
			switch (msaa)
			{
			case MSAAMODE_OFF:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = false;
				break;
			case MSAAMODE_SAMPLE_COUNT_1:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_1_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_2:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_2_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_4:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_4_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_8:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_8_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_16:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_16_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_32:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_32_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_64:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_64_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_BEST:
				mVulkanEngine->GetVulkanBaseStruct().UseMSAA = true;
				mVulkanEngine->GetVulkanBaseStruct().MsaaFlagBits = Invision::GetMaxUsableSampleCount(mVulkanEngine->GetVulkanBaseStruct());
				break;
			}
		}
		else
		{
			mVulkanEngine->GetVulkanBaseStruct().UseMSAA = false;
		}

	}

	void VulkanInstance::UpdateDepthTexture()
	{
		if (mDepthRessources.GetImageView() != VK_NULL_HANDLE)
		{
			mDepthRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
		}

		mDepthRessources.CreateDepthRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
		mUseDepthTest = true;
		mVulkanContext.mUseDepthRessources = true;
	}

	void VulkanInstance::UpdateMSAATexture()
	{
		if (mColorRessources.GetImageView() != VK_NULL_HANDLE)
		{
			mColorRessources.DestroyTexture(mVulkanEngine->GetVulkanBaseStruct());
		}

		mColorRessources.CreateColorRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanEngine->GetCommandPool(), mVulkanEngine->GetMemoryManager(), mVulkanContext);
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

	std::shared_ptr<ITexture> VulkanInstance::CreateTexture(unsigned char* pixels, int width, int height, bool generateMipMaps)
	{
		return std::make_shared<VulkanTexture>(this, pixels, width, height, generateMipMaps);
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