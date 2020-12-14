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

	VulkanInstance::VulkanInstance(VulkanEngine* engine, CanvasDimensions dimensions, bool activateDepthTest)
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
	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas)
	{
		Invision::DestroyPresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext);
		Invision::CreatePresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext, canvas.width, canvas.height);

		if (mUseDepthTest)
		{
			depthRessources.DestroyDepthRessources(vulkanEngine->GetVulkanInstance());
			depthRessources.CreateDepthRessources(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), vulkanContext);
		}
	}

	void VulkanInstance::ResetPresentation(CanvasDimensions canvas, bool activateDepthTest)
	{
		Invision::DestroyPresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext);
		Invision::CreatePresentationSystem(vulkanEngine->GetVulkanInstance(), vulkanContext, canvas.width, canvas.height);

		if (activateDepthTest)
		{
			depthRessources.DestroyDepthRessources(vulkanEngine->GetVulkanInstance());
			depthRessources.CreateDepthRessources(vulkanEngine->GetVulkanInstance(), vulkanEngine->GetCommandPool(), vulkanContext);
			mUseDepthTest = true;
		}
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

	std::shared_ptr<IPushConstant> VulkanInstance::CreatePushConstant(ShaderStage shaderStages, uint32_t offset, uint32_t size)
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