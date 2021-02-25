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

		renderPass = mMainRenderPass;
	
		mMainFramebuffer = std::make_shared<VulkanFramebuffer>(this, renderPass, true);
		mMainCommandBuffer = CreateCommandBuffer(mMainFramebuffer);

		
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
		mMainFramebuffer = std::make_shared<VulkanFramebuffer>(this, renderPass, true);

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

	std::shared_ptr<ITexture> VulkanInstance::CreateTexture(unsigned char* pixels, int width, int height, GfxFormat format, bool generateMipMaps)
	{
		return std::make_shared<VulkanTexture>(this, pixels, width, height, format, generateMipMaps);
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

	VkFormat VulkanInstance::ConvertInvisionFormatToVkFormat(GfxFormat format)
	{

		VkFormat vkformat;

		switch (format)
		{
		case FORMAT_UNEDFINED:
			vkformat = VK_FORMAT_UNDEFINED;
			break;
		case FORMAT_R32G32B32A32_SFLOAT:
			vkformat = VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case FORMAT_R32G32B32A32_UINT:
			vkformat = VK_FORMAT_R32G32B32A32_UINT;
			break;
		case FORMAT_R32G32B32A32_SINT:
			vkformat = VK_FORMAT_R32G32B32A32_SINT;
			break;
		case FORMAT_R32G32B32_SFLOAT:
			vkformat = VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case FORMAT_R32G32B32_UINT:
			vkformat = VK_FORMAT_R32G32B32_UINT;
			break;
		case FORMAT_R32G32B32_SINT:
			vkformat = VK_FORMAT_R32G32B32_SINT;
			break;
		case FORMAT_R16G16B16A16_SFLOAT:
			vkformat = VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case FORMAT_R16G16B16A16_UNORM:
			vkformat = VK_FORMAT_R16G16B16A16_UNORM;
			break;
		case FORMAT_R16G16B16A16_UINT:
			vkformat = VK_FORMAT_R16G16B16A16_UINT;
			break;
		case FORMAT_R16G16B16A16_SNORM:
			vkformat = VK_FORMAT_R16G16B16A16_SNORM;
			break;
		case FORMAT_R16G16B16A16_SINT:
			vkformat = VK_FORMAT_R16G16B16A16_SINT;
			break;
		case FORMAT_R32G32_SFLOAT:
			vkformat = VK_FORMAT_R32G32_SFLOAT;
			break;
		case FORMAT_R32G32_UINT:
			vkformat = VK_FORMAT_R32G32_UINT;
			break;
		case FORMAT_R32G32_SINT:
			vkformat = VK_FORMAT_R32G32_SINT;
			break;
		case FORMAT_R8G8B8A8_UNORM:
			vkformat = VK_FORMAT_R8G8B8A8_UNORM;
			break;
		case FORMAT_R8G8B8A8_SRGB:
			vkformat = VK_FORMAT_R8G8B8A8_SRGB;
			break;
		case FORMAT_R8G8B8A8_UINT:
			vkformat = VK_FORMAT_R8G8B8A8_UINT;
			break;
		case FORMAT_R8G8B8A8_SNORM:
			vkformat = VK_FORMAT_R8G8B8A8_SNORM;
			break;
		case FORMAT_R8G8B8A8_SINT:
			vkformat = VK_FORMAT_R8G8B8A8_SINT;
			break;
		case FORMAT_R16G16_SFLOAT:
			vkformat = VK_FORMAT_R16G16_SFLOAT;
			break;
		case FORMAT_R16G16_UNORM:
			vkformat = VK_FORMAT_R16G16_UNORM;
			break;
		case FORMAT_R16G16_UINT:
			vkformat = VK_FORMAT_R16G16_UINT;
			break;
		case FORMAT_R16G16_SNORM:
			vkformat = VK_FORMAT_R16G16_SNORM;
			break;
		case FORMAT_R16G16_SINT:
			vkformat = VK_FORMAT_R16G16_SINT;
			break;
		case FORMAT_D32_SFLOAT:
			vkformat = VK_FORMAT_D32_SFLOAT;
			break;
		case FORMAT_R32_SFLOAT:
			vkformat = VK_FORMAT_R32_SFLOAT;
			break;
		case FORMAT_R32_UINT:
			vkformat = VK_FORMAT_R32_UINT;
			break;
		case FORMAT_R32_SINT:
			vkformat = VK_FORMAT_R32_SINT;
			break;
		case FORMAT_D24_UNORM_S8_UINT:
			vkformat = VK_FORMAT_D24_UNORM_S8_UINT;
			break;
		case FORMAT_R8G8_UNORM:
			vkformat = VK_FORMAT_R8G8_UNORM;
			break;
		case FORMAT_R8G8_UINT:
			vkformat = VK_FORMAT_R8G8_UINT;
			break;
		case FORMAT_R8G8_SNORM:
			vkformat = VK_FORMAT_R8G8_SNORM;
			break;
		case FORMAT_R8G8_SINT:
			vkformat = VK_FORMAT_R8G8_SINT;
			break;
		case FORMAT_R16_SFLOAT:
			vkformat = VK_FORMAT_R16_SFLOAT;
			break;
		case FORMAT_D16_UNORM:
			vkformat = VK_FORMAT_D16_UNORM;
			break;
		case FORMAT_R16_UNORM:
			vkformat = VK_FORMAT_R16_UNORM;
			break;
		case FORMAT_R16_UINT:
			vkformat = VK_FORMAT_R16_UINT;
			break;
		case FORMAT_R16_SNORM:
			vkformat = VK_FORMAT_R16_SNORM;
			break;
		case FORMAT_R16_SINT:
			vkformat = VK_FORMAT_R16_SINT;
			break;
		case FORMAT_R8_UNORM:
			vkformat = VK_FORMAT_R8_UNORM;
			break;
		case FORMAT_R8_UINT:
			vkformat = VK_FORMAT_R8_UINT;
			break;
		case FORMAT_R8_SNORM:
			vkformat = VK_FORMAT_R8_SNORM;
			break;
		case FORMAT_R8_SINT:
			vkformat = VK_FORMAT_R8_SINT;
			break;
		case FORMAT_BC6H_UFLOAT:
			vkformat = VK_FORMAT_BC6H_UFLOAT_BLOCK;
			break;
		case FORMAT_BC6H_SFLOAT:
			vkformat = VK_FORMAT_BC6H_SFLOAT_BLOCK;
			break;
		case FORMAT_BC7_UNORM:
			vkformat = VK_FORMAT_BC7_UNORM_BLOCK;
			break;
		case FORMAT_BC7_SRGB:
			vkformat = VK_FORMAT_BC7_SRGB_BLOCK;
			break;
		default:
			vkformat = VK_FORMAT_UNDEFINED;
		}


		return vkformat;
	}

}