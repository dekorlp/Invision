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

	VulkanInstance::VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, std::shared_ptr <Invision::IRenderPass>& renderPass, std::shared_ptr <Invision::IFramebuffer>& framebuffer, std::shared_ptr <Invision::ICommandBuffer>& commandBuffer, MSAAMode msaaMode)
		: IGraphicsInstance(engine)
	{
		mVulkanEngine = engine;

		ActivateMSAA(msaaMode);
		
		
		Invision::VulkanBaseDevice().CreateLogicalDevice(engine->GetVulkanBaseStruct(), mVulkanContext);
		mCommandPool.CreateCommandPool(mVulkanContext);
		mMemoryManager.Init(engine->GetVulkanBaseStruct(), mVulkanContext, 2147483648); // Allocate 2GB

		Invision::CreateSurface(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.hwnd);
		Invision::CreatePresentationSystem(engine->GetVulkanBaseStruct(), mVulkanContext, dimensions.width, dimensions.height);

		

		// Create Default RenderPass / FrameBuffer / CommandBuffer
		mMainRenderPass = CreateRenderPass();
		std::vector<Invision::VulkanBaseTexture> frameBufferAttachments;
		
		// Create Depth Ressources
		UpdateDepthTexture();


		// Create Color Ressources for Multisampling
		if (mVulkanContext.UseMSAA)
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
		Invision::DestroyPresentationSystem(mVulkanContext);
		Invision::CreatePresentationSystem(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext, canvas.width, canvas.height);

		if (mVulkanContext.UseMSAA == true)
		{
			UpdateMSAATexture();
		}

		
		UpdateDepthTexture();

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
				mVulkanContext.UseMSAA = false;
				break;
			case MSAAMODE_SAMPLE_COUNT_1:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_1_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_2:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_2_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_4:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_4_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_8:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_8_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_16:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_16_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_32:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_32_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_64:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::IsMSAASampleSupported(mVulkanEngine->GetVulkanBaseStruct(), VK_SAMPLE_COUNT_64_BIT);
				break;
			case MSAAMODE_SAMPLE_COUNT_BEST:
				mVulkanContext.UseMSAA = true;
				mVulkanContext.MsaaFlagBits = Invision::GetMaxUsableSampleCount(mVulkanEngine->GetVulkanBaseStruct());
				break;
			}
		}
		else
		{
			mVulkanContext.UseMSAA = false;
		}

	}

	void VulkanInstance::UpdateDepthTexture()
	{
		if (mDepthRessources.GetImageView() != VK_NULL_HANDLE)
		{
			mDepthRessources.DestroyTexture(mVulkanContext);
		}


		mDepthRessources.CreateDepthRessources(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext, mCommandPool, mMemoryManager, mVulkanContext.swapChainExtent.width, mVulkanContext.swapChainExtent.height, mVulkanContext.MsaaFlagBits, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

	}

	void VulkanInstance::UpdateMSAATexture()
	{
		if (mColorRessources.GetImageView() != VK_NULL_HANDLE)
		{
			mColorRessources.DestroyTexture(mVulkanContext);
		}

		mColorRessources.CreateColorRessources(mVulkanContext, mCommandPool, mMemoryManager, mVulkanContext.swapChainExtent.width, mVulkanContext.swapChainExtent.height, mVulkanContext.MsaaFlagBits, mVulkanContext.swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
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

	std::shared_ptr<IRenderPass> VulkanInstance::CreateDepthOnlyRenderPass(std::shared_ptr<ITexture>& attachment)
	{
		std::shared_ptr< VulkanRenderPass> renderPass = std::make_shared<VulkanRenderPass>(this);
		dynamic_pointer_cast<VulkanRenderPass>(renderPass)->CreateDepthOnlyRenderPass(attachment);

		return  renderPass;
	}

	std::shared_ptr<IVertexBindingDescription> VulkanInstance::CreateVertexBindingDescription()
	{
		return  std::make_shared<VulkanVertexBindingDescription>();
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

	std::shared_ptr<IFramebuffer> VulkanInstance::CreateFramebuffer(std::shared_ptr<IRenderPass> renderPass, int width, int height)
	{
		return std::make_shared<VulkanFramebuffer>(this, renderPass, width, height);
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

	std::shared_ptr<ITexture> VulkanInstance::CreateColorAttachment(int width, int height, GfxFormat format)
	{
		std::shared_ptr<ITexture> colorAttachment = std::make_shared<VulkanTexture>(this);
		colorAttachment->CreateColorAttachment(width, height, format);
		return colorAttachment;


	}

	std::shared_ptr<ITexture> VulkanInstance::CreateDepthAttachment(int width, int height, bool isDepthStencil)
	{
		std::shared_ptr<ITexture> depthAttachment = std::make_shared<VulkanTexture>(this);
		depthAttachment->CreateDepthAttachment(width, height, isDepthStencil);
		return depthAttachment;
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
		if (mVulkanContext.UseMSAA == true)
		{
			mColorRessources.DestroyTexture(mVulkanContext);
		}

		mDepthRessources.DestroyTexture(mVulkanContext);

		Invision::DestroyPresentationSystem(mVulkanContext);
		Invision::DestroySurface(mVulkanEngine->GetVulkanBaseStruct(), mVulkanContext);

		mMemoryManager.Destroy(mVulkanContext);
		mCommandPool.DestroyCommandPool(mVulkanContext);
		//Invision::DestroyPresentationSystem(vulkInstance, vulkanContext);
		Invision::DestroyVulkanDevice(mVulkanContext);
	}

	VulkanBaseMemoryManager& VulkanInstance::GetMemoryManager()
	{
		return mMemoryManager;
	}


	Invision::VulkanBaseCommandPool VulkanInstance::GetCommandPool()
	{
		return mCommandPool;
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